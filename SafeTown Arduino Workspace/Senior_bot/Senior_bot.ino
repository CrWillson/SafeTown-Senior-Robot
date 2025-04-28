#include "debug_logger.hpp"
#include "event_manager.hpp"
#include "display.hpp"
#include "user_input.hpp"
#include "esp32_input.hpp"
#include "route_manager.hpp"
#include "sr_bot_menu.hpp"

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include <Servo.h>
#include <Motor.h>
#include <unordered_map>
#include <string>
#include <array>
#include <algorithm>
#include <numeric>
#include <FS.h>
#include <LittleFS.h>


#define DIST_MAX    28
#define DIST_MIN   -28
#define DIST_THRES  24
#define DIST_CENTER  0

#define STOP     17
#define START    16
#define DISP_SDA  4
#define DISP_SCL  5
#define LEFT_A   24
#define LEFT_B   23
#define RIGHT_A  22
#define RIGHT_B  21
#define RED      10
#define GREEN    11
#define YELLOW   12
#define BUZZER   13
#define SERVO    14
#define ENC_B    20
#define ENC_A    18
#define ENC_S    19
#define BRAKE_LIGHT    25 //also BUILTIN_LED
#define TURN_L    2
#define TURN_R    3
#define TX        1

Servo steer;
constexpr int left_turn_max = 77;
constexpr int left_turn_rad = 65;
constexpr int center = 90;
constexpr int right_turn_rad = 65;
constexpr int right_turn_max = 77;
long pos;     // The turn angle of the steering servo. Smaller number means more right.

enum DIR {
  STRAIGHT,
  LEFT,
  RIGHT
};

int gps_i = 0;                  // The current step in the route the car is on
constexpr std::array gps = {    // The route being followed
  DIR::LEFT, 
  DIR::RIGHT, 
  DIR::LEFT,
  DIR::STRAIGHT,
};    
constexpr auto gps_size = gps.size();

const std::unordered_map<DIR, std::string> DirLabel {   // Map to convert from an enum direction to a char arrow
  {DIR::STRAIGHT, "^"},
  {DIR::LEFT, "<"},
  {DIR::RIGHT, ">"}
};

long oldTime = 0;       // Used to keep track of the time when the current state was entered
long led_time = 0;
long obstacleTime = 0;  // Used to keep track of the time when an obstacle was detected
long oldTimeDiff = 0;

int8_t dist = 0;              // Distance to the white line
bool stop_detected = false;   // Was a stop detected
bool car_detected = false;    // Was an obstacle detected
bool bulldozerMode = false;   // Should obstacles and stops be ignored

Motor left_motor(LEFT_A, LEFT_B);
Motor right_motor(RIGHT_A, RIGHT_B);
const int SPEED = 100;        // Speed of the motors
bool brake = true;            // Should the car stop
bool leftTurnSig = false;     // Should the left turn signal be on
bool rightTurnSig = false;    // Should the right turn signal be on

// long error;                               // Error in white distance. Used for PID control
// std::array<long, 167> error_history{0};   // History of error measurements
// int error_hi = 0;

enum State : uint8_t {        // All states the car can be in
  STOPPED = 0,
  FOLLOWING = 1,
  CLOSE = 2,
  FAR = 3,
  INTERSECTION_WAIT = 4,
  INTERSECTION_STRAIGHT = 5,
  INTERSECTION_RIGHT = 6,
  INTERSECTION_LEFT = 7,
  TRAFFIC = 8,
  PARKED = 9,
};

volatile State state = State::STOPPED;    // The current state
State oldState = State::STOPPED;          // The previous state

EventManager& eventManager = EventManager::getInstance();   // Event manager instantiation. Hanldes message passing between components
ESP32& esp32 = ESP32::getInstance();                        // ESP32 communication manager
UIManager& uimanager = UIManager::getInstance();            // User input manager. Specifically handles the rotary encoder
Display display;                                            // Display manager. Handles drawing to the screen
SrMenu menu;                                                // Concrete instantiation of a menu specifically for the senior robot


void setup1() {
  // Start the event manager processing loop running on CORE 1
  eventManager.processEvents();
}

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin()) {
    LOGLN("LittleFS failed to mount!");
    return;
  }

  pinMode(START, INPUT);
  pinMode(STOP, INPUT);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(BRAKE_LIGHT, OUTPUT);
  pinMode(TURN_L, OUTPUT);  
  pinMode(TURN_R, OUTPUT);
  digitalWrite(BRAKE_LIGHT, LOW);
  digitalWrite(TURN_L, LOW);
  digitalWrite(TURN_R, LOW);

  attachInterrupt(digitalPinToInterrupt(START), startISR, RISING);
  attachInterrupt(digitalPinToInterrupt(STOP), haltISR, RISING);

  esp32.init("/images/");
  uimanager.init();
  menu.init();
  display.init();

  steer.attach(SERVO);

  left_motor.begin();
  right_motor.begin();

  // Delay to give the ESP32 time to start up
  delay(3000);
}


void loop() {

  // Block and wait for a packet from the ESP32
  auto packet = esp32.receivePacket();
  
  // Extract information from the packet
  dist = packet.whiteDist;
  stop_detected = packet.stopDetected;

  // Publish events to notify other components of the new values
  auto distUpdate = Event::ValueChangedEvent("whiteDist", std::to_string(dist));
  eventManager.publish(distUpdate);

  std::string stop_str = (stop_detected) ? "T" : "F";
  auto stopUpdate = Event::ValueChangedEvent("stopDetect", stop_str);
  eventManager.publish(stopUpdate);

  auto steerUpdate = Event::ValueChangedEvent("steerAngle", std::to_string(pos));
  eventManager.publish(steerUpdate);

  auto turnUpdate = Event:: ValueChangedEvent("nextTurn", DirLabel.at(gps[gps_i]));
  eventManager.publish(turnUpdate);


  //******************************
  //  BEGIN MAIN STATE MACHINE
  //******************************
  stateLEDs(state);
  switch (state) {
    //-------------------------------------------------------------------------------------
    //  STANDARD LINE FOLLOWING
    //-------------------------------------------------------------------------------------
    case State::FOLLOWING:
      brake = false;
      leftTurnSig = false;
      rightTurnSig = false;
      //maps the dist factor to the turn radius and adjust    
      // error = map_pos(dist) - center;
      // pos = center + pid_controller(error);
      
      pos = map_pos(dist);
      steer.write(pos);
      //Serial.println(pos);
      
      //checks if we have lost the line
      if (abs(dist) > DIST_THRES) {
        //and which side we lost it to
        if (dist < DIST_CENTER) {
          state = State::CLOSE;
        } else {
          state = State::FAR;
        }
      } else if (car_detected && !bulldozerMode) {
        oldState = state;
        state = State::TRAFFIC;
        obstacleTime = millis();
      } else if (stop_detected) {
        oldTime = millis();
        state = State::INTERSECTION_WAIT;
      }

      break;

    //-------------------------------------------------------------------------------------
    //  LINE TOO FAR AWAY
    //-------------------------------------------------------------------------------------
    case State::FAR:
      brake = false;
      leftTurnSig = false;
      rightTurnSig = false;
      //turns as HARD as possible toward the line
      //turns towards line
      pos = center + right_turn_rad;
      steer.write(pos);

      //checks if we are back in range to continue following the line normally
      if (abs(dist) < DIST_THRES) {
        state = State::FOLLOWING;
      } else if (car_detected && !bulldozerMode) {
        oldState = state;
        state = State::TRAFFIC;
        obstacleTime = millis();
      } else if (stop_detected) {
        oldTime = millis();
        state = State::INTERSECTION_WAIT;
      }
      break;

    //-------------------------------------------------------------------------------------
    //  LINE TOO CLOSE
    //-------------------------------------------------------------------------------------
    case State::CLOSE:
      brake = false;
      leftTurnSig = false;
      rightTurnSig = false;

      //turns as HARD as possible away from the line
      pos = center - left_turn_rad;
      steer.write(pos);

      //checks if we are back in range to continue following the line normally
      if (abs(dist) < DIST_THRES) {
        state = State::FOLLOWING;
      } else if (car_detected && !bulldozerMode) {
        oldState = state;
        state = State::TRAFFIC;
        obstacleTime = millis();
      } else if (stop_detected) {
        oldTime = millis();
        state = State::INTERSECTION_WAIT;
      }
      break;

    //-------------------------------------------------------------------------------------
    //  HALT THE CAR
    //-------------------------------------------------------------------------------------
    case State::STOPPED:
      //stop the robot
      // error = 0;
      brake = true;
      leftTurnSig = false;
      rightTurnSig = false;
      steer.write(center);
      digitalWrite(BUZZER, LOW);
      gps_i = 0; //resets the path, might not be desired functionality.
                 //treats STOPPED as a stop, instead of a pause.
                 //remove these lines if you want a pause.
      break;

    //-------------------------------------------------------------------------------------
    //  WAIT BEFORE ENTERING INTERSECTION
    //-------------------------------------------------------------------------------------
    case State::INTERSECTION_WAIT:
      //WAIT at intersection
      brake = true;
      leftTurnSig = gps[gps_i] == DIR::LEFT;
      rightTurnSig = gps[gps_i] == DIR::RIGHT;

      // Wait for intersection to be clear
      if (car_detected && !bulldozerMode) {
        oldState = state;
        state = State::TRAFFIC;
        obstacleTime = millis();
      } else if (millis() > oldTime + 1000) { //otherwise, wait 1 second and then GO GO GO!
        oldState = state;
        oldTime = millis();

        if (gps[gps_i] == DIR::RIGHT) {
          state = State::INTERSECTION_RIGHT;
        }
        else if (gps[gps_i] == DIR::LEFT) {
          state = State::INTERSECTION_LEFT;
        }
        else if (gps[gps_i] == DIR::STRAIGHT) {
          state = State::INTERSECTION_STRAIGHT;
        }
      }
      break;

    //-------------------------------------------------------------------------------------
    //  DRIVE STRIAGHT THROUGH INTERSECTION
    //-------------------------------------------------------------------------------------
    case INTERSECTION_STRAIGHT:
      brake = false;
      leftTurnSig = false;
      rightTurnSig = false;
      
      if (car_detected && !bulldozerMode) { 
        oldState = state;
        state = State::TRAFFIC;
        obstacleTime = millis();
        oldTimeDiff = millis() - oldTime;
      } else {
        const int endTime = oldTime + 1000; // The time to resume line following

        // The car starts by going roughly forward and a little left
        pos = center - 2;
        steer.write(pos);

        // Car has gone forward enough. Resume line following
        if (millis() > endTime) {
          oldTime = millis();
          oldState = state;
          state = State::FOLLOWING;
          gps_i = (gps_i + 1) % gps_size;
        }
      }

      break;
    
    //-------------------------------------------------------------------------------------
    //  TURN RIGHT IN INTERSECTION
    //-------------------------------------------------------------------------------------
    case INTERSECTION_RIGHT:
      brake = false;
      leftTurnSig = false;
      rightTurnSig = true;
      
      if (car_detected && !bulldozerMode) { 
        oldState = state;
        state = State::TRAFFIC;
        obstacleTime = millis();
        oldTimeDiff = millis() - oldTime;
      } else {
        const int turnTime = oldTime + 300;   // The time to begin turning right
        const int endTime = turnTime + 1700;   // The time to resume line following

        // The car starts by going forward just a little bit
        pos = center;
        steer.write(pos);

        // After a bit the car turns hard right
        if (millis() > turnTime) {
          steer.write(center + right_turn_max);
        }

        // The car has turned right long enough, resume line following
        if (millis() > endTime) {
          oldTime = millis();
          oldState = state;
          state = State::FOLLOWING;
          gps_i = (gps_i + 1) % gps_size;
        }
      }
      break;
    
    //-------------------------------------------------------------------------------------
    //  TURN LEFT IN INTERSECTION
    //-------------------------------------------------------------------------------------
    case INTERSECTION_LEFT:
      brake = false;
      leftTurnSig = true;
      rightTurnSig = false;
      
      if (car_detected && !bulldozerMode) { 
        oldState = state;
        state = State::TRAFFIC;
        obstacleTime = millis();
        oldTimeDiff = millis() - oldTime;
      } else {
        const int turnTime = oldTime + 1300;          // The time to begin turning left
        const int stopTurnTime = turnTime + 1700;     // The time to stop turning, go forward a little more
        const int endTime = stopTurnTime + 500;       // The time to resume line following

        // The car starts by going roughly forward and a little left
        pos = center - 10;
        steer.write(pos);

        // After a bit the car turns hard left
        if (millis() > turnTime) {
          steer.write(center - left_turn_max);
        }

        // Go straight for just a little bit
        if (millis() > stopTurnTime) {
          pos = center;
          steer.write(pos);
        }

        // The car has turned left long enough, resume line following
        if (millis() > endTime) {
          oldTime = millis();
          oldState = state;
          state = State::FOLLOWING;
          gps_i = (gps_i + 1) % gps_size;
        }
      }
      break;

    //-------------------------------------------------------------------------------------
    //  STOP TEMPORARILY FOR TRAFFIC
    //-------------------------------------------------------------------------------------
    case TRAFFIC:
      //SET oldTime = millis(); IN THE STATE JUST BEFORE ENTERING, like in FOLLOWING
      brake = true;
      leftTurnSig = leftTurnSig; //keep these the same
      rightTurnSig = rightTurnSig; //left here for symbolic purposes, the compiler gets rid of these lines.
      //if the road is clear, wait 0.75s before going forward
      if (millis() > oldTime + 750) {
        state = oldState;
      } else if (false) { // replace this with a "car detected in front" variable
        oldTime = millis();
      }
      break;
    default: 
      //IF THE ROBOT IS HERE, SOMETHING IS VERY WRONG
      //in any other state, stop the robot and turn on the buzzer
      brake = true;
      leftTurnSig = true;
      rightTurnSig = true;
      steer.write(center);
      analogWrite(BUZZER, 30);
      break;

  }
  //****************************
  //  END MAIN STATE MACHINE
  //****************************
  
  // Apply the brakes 
  if(brake) {
    left_motor.brake();
    right_motor.brake();
  } else {
    left_motor.forward(SPEED);
    right_motor.forward(SPEED);
  }



  //LED Control

  //brake lights are on if we are braking
  if(brake) {
    digitalWrite(BRAKE_LIGHT, HIGH);
  } else {
    digitalWrite(BRAKE_LIGHT, LOW);
  }

  //left lights blink on and off with a period of 1 second, otherwise are off
  if (leftTurnSig && millis() % 1000 > 500) {
    digitalWrite(TURN_L, HIGH);
  } else {
    digitalWrite(TURN_L, LOW);
  }

  //right lights blink on and off with a period of 1 second, otherwise are off
  if (rightTurnSig && millis() % 1000 > 500) {
    digitalWrite(TURN_R, HIGH);
  } else {
    digitalWrite(TURN_R, LOW);
  }

  //keeps a history of error for 2 purposes:
  //1.) D and I components of the PID controller
  //2.) allows us to adjust our turn angle at an
  //    intersection based on the angle of the robot
  // error_history[error_hi] = error;
  // error_hi = (error_hi + 1) % error_history.size();
  
}



//----------------------------------------------------------------------------------------------------------
// BEGIN OTHER FUNCTIONS
//----------------------------------------------------------------------------------------------------------
void haltISR() {
  state = State::STOPPED;
}

void startISR() {
  state = State::FOLLOWING;
}

// Function to handle saving the previous state and state entry time
void change_state(State newState) {
  oldTime = millis();
  oldState = state;
  state = newState;
}

// long pid_controller(long error) {
//   /* The PID Controller that defines line following
//    * Proportional: error used for e(t)
//    * Integral: NO INTEGRAL COMPONENT (maybe add? I'm not the best with controllers, this project is my only experience)
//    * Derivative: error - avg(error_history) is used as an estimate of the de(t)/dt.
//    * NOTE: NONE OF THE VALUES ARE LIKELY OPTIMAL
//    */
//   //             Kp * e(t)  + Kd * de(t)/dt
//   //                                VVV is this supposed to be error or steer.read()?
//   return 5 * (error) / 8 - ((5  * (error - avg(error_history))) / 4);
// }

template <typename T, std::size_t N>
long avg(const std::array<T, N>& arr) {
    long sum = std::accumulate(arr.begin(), arr.end(), 0L);

    return (N > 0) ? (sum / static_cast<long>(N)) : 0;
}

void stateLEDs(int state) {
  digitalWrite(RED, (state >> 2) & 0x01);
  digitalWrite(GREEN, (state >> 1) & 0x1);
  digitalWrite(YELLOW, (state) & 0x1);
}

int map_pos(int dist) {
  return map(dist, DIST_MAX, DIST_MIN, center + left_turn_rad, center - right_turn_rad);
}

int in_speed(int servo_pos) {
  const float wheel_radius = 10; //(mm)
  const float wheel_base = 86; //(mm), measured from front wheel axle to rear wheel axle
  const float wheel_dist = 40; //(mm), measure from inside of front right wheel to inside of front left wheel

  float turn_radius;
  turn_radius = wheel_base / tan(steering_angle(servo_pos)) - wheel_dist / 2;

  //w * r = v <-- vehicle speed (const)
  //so w_turn = w_wheel * r_wheel / r_turn

  float w_turn;
  w_turn = SPEED * wheel_radius / turn_radius;
  return SPEED + (int)w_turn;
}

int out_speed(int servo_pos) { //same as in_speed, but turn_radius is + wheel_dist / 2 instead of -
  const float wheel_radius = 10; //(mm)
  const float wheel_base = 86; //(mm), measured from front wheel axle to rear wheel axle
  const float wheel_dist = 40; //(mm), measure from inside of front right wheel to inside of front left wheel

  float turn_radius;
  turn_radius = wheel_base / tan(steering_angle(servo_pos)) + wheel_dist / 2;

  //w * r = v <-- vehicle speed (const)
  //so w_turn = w_wheel * r_wheel / r_turn

  float w_turn;
  w_turn = SPEED * wheel_radius / turn_radius;
  Serial.println(w_turn);
  return SPEED + (int)w_turn;
}


float steering_angle(int servo_pos) { //returns IN RADIANS
  //THIS MIGHT NOT BE CORRECT, JUST LEAVING THIS HERE FOR NOW
  //71/4068 is an approximation of pi/180, accurate to 8 decimal places
  return (servo_pos - center) * 71 / 4068.0;
}
