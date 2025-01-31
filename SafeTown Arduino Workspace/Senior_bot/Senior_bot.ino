#include <Servo.h>
#include <Motor.h>
#include "display.hpp"

#define DIST_MAX    22
#define DIST_MIN   -22
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
long pos;
long pos_history[50] = {0};
constexpr int pos_history_size = sizeof(pos_history) / sizeof(pos_history[0]);
int pos_hi = 0;


enum DIR {
  STRAIGHT,
  LEFT,
  RIGHT
};

int gps_i = 0;
constexpr int gps[] = {DIR::STRAIGHT, DIR::LEFT, DIR::RIGHT, DIR::STRAIGHT};
constexpr int gps_size = sizeof(gps) / sizeof(gps[0]);

long cycle = 0;
long oldTime = 0;
long led_time = 0;
long obstacleTime = 0;
long oldTimeDiff = 0;

constexpr int left_turn_rad = 65;
constexpr int center = 85;
constexpr int right_turn_rad = 65;

int8_t dist = 0;
bool stop_detected = false;
bool car_detected = false;
bool bulldozerMode = false;
long error;

Motor left_motor(LEFT_A, LEFT_B);
Motor right_motor(RIGHT_A, RIGHT_B);
const int SPEED = 100;
bool brake = true;
bool leftTurnSig = false;
bool rightTurnSig = false;

long error_history[167] = {0}; //size needs to be hardcoded to the avg() function, if changed here, change there
constexpr int error_history_size = sizeof(error_history) / sizeof(error_history[0]);
int error_hi = 0;

enum State {
  STOPPED = 0,
  FOLLOWING = 2,
  CLOSE = 1,
  FAR = 4,
  INTERSECTION_WAIT = 3,
  INTERSECTION_NAV = 5,
  INTERSECTION_FOR = 6,
  TRAFFIC = 7,
  PARKED = 8,
}
volatile state = State::STOPPED;
State oldState = State::STOPPED;

Display display = Display();
std::vector<std::string> testLines = {"Hello", "Line2", "Line3", "World!"};

void setup() {
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

  display.clearDisplay();

  steer.attach(SERVO);

  left_motor.begin();
  right_motor.begin();
  Serial1.setPollingMode(true);
  Serial1.begin(500000);  //serial for the UART connection to the ESP32 CAM

  Serial.begin(500000);   // serial for serial monitor
}

void loop() {
  if (cycle > 10000) {
    cycle = 0;
    display.printLines(testLines);
  }

  if(Serial1.available()) {
    //dist = Serial1.read();
    String str = Serial1.readStringUntil('\n');
    Serial.println(str);
  
    stop_detected = str[1] == '1';
    //car_detected = str[0] == '1';
    dist = 0;
    for (int i = 2; i < 10; i++) {
      dist = (dist << 1) | (str[i] - '0');
    }
    dist = -dist;
    Serial.println(dist);
    Serial.println(stop_detected);
    Serial.println(car_detected);

  }
  
  stateLEDs(state);
  switch (state) {
    case State::FOLLOWING:
      brake = false;
      leftTurnSig = false;
      rightTurnSig = false;
      //maps the dist factor to the turn radius and adjust    
      error = map_pos(dist) - center;

      pos = center + pid_controller(error);
      steer.write(pos);
      //Serial.println(pos);
      
      //checks if we have lost the line
      if (abs(dist) > DIST_THRES) {
        //and which side we lost it to
        if (dist > DIST_CENTER) {
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
    case State::FAR:
      brake = false;
      leftTurnSig = false;
      rightTurnSig = false;
      //turns as HARD as possible toward the line
      //turns towards line
      error = right_turn_rad;
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
    case State::CLOSE:
      brake = false;
      leftTurnSig = false;
      rightTurnSig = false;

      //turns as HARD as possible away from the line
      error = -left_turn_rad;
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
    case State::STOPPED:
      //stop the robot
      error = 0;
      brake = true;
      leftTurnSig = false;
      rightTurnSig = false;
      steer.write(center);
      digitalWrite(BUZZER, LOW);
      gps_i = 0; //resets the path, might not be desired functionality.
                 //treats STOPPED as a stop, instead of a pause.
                 //remove these lines if you want a pause.
      break;
    case State::INTERSECTION_WAIT:
      //WAIT at intersection
      brake = true;
      leftTurnSig = gps[gps_i] == 1;
      rightTurnSig = gps[gps_i] == 2;

      // Wait for intersection to be clear
      if (car_detected && !bulldozerMode) {
        oldState = state;
        state = State::TRAFFIC;
        obstacleTime = millis();
      } else if (millis() > oldTime + 1000) { //otherwise, wait 1 second and then GO GO GO!
        oldTime = millis();
        if (gps[gps_i] == DIR::RIGHT) {
          state = State::INTERSECTION_NAV;
        } else {
          state = State::INTERSECTION_FOR;
        }
      }
      break;

    case INTERSECTION_FOR:
      brake = false;
      //SET oldTime = millis(); when entering this state
      //go forward to line up the back wheels with the stop line before turning
      error = 3 * avg(error_history, error_history_size) / 4; // keep same error
      pos = center + error;
      steer.write(pos);
      if (car_detected && !bulldozerMode) { 
        oldState = state;
        state = State::TRAFFIC;
        obstacleTime = millis();
        oldTimeDiff = millis() - oldTime;
      } else if (millis() > oldTime + 1000) {
        oldTime = millis();
        state = State::INTERSECTION_NAV;
      }
      break;
    
    case INTERSECTION_NAV:
      brake = false;
      leftTurnSig = gps[gps_i] == 1;
      rightTurnSig = gps[gps_i] == 2;
      /* NOTE: All of these cases include an -avg(error_history)
       *       This predicts the angle we have come from to adjust for entering intersections after curves
       * NOT CURRENTLY, I found it to be unreliable, at least straight as -avg(eh), no adjustments. So some work needs to be done here
      */
      {
      int turn_time = 0;

      switch (gps[gps_i]) {
        case DIR::STRAIGHT: //straight
          //                 VVVV this adjustment factor is TEMPORARY - it should be replaced with the negative average of the position history
          steer.write(center - 5);
          turn_time = 500;
          break;
        case DIR::LEFT: //left
          //THIS IS WRONG, USE AVG OF HISTORY INSTEAD
          //int theta;
          //theta = center - prev_pos; //angle of servo is inverse angle of robot b/c it's trying to correct

          //if under turning (going off on the right of the line), increase the number
          //if over  turning (going off on the left  of the line), decrease the number
          //                   VV this is the number, the comments above ignore the negative 
          steer.write(center - left_turn_rad);
          turn_time = 1500;
          break;
        case DIR::RIGHT: //right
          //anchors right wheel
          steer.write(center + right_turn_rad);
          turn_time = 500;
          break;
        default:
          break;
      }
      //makes us wait 0.25 seconds before being able to switch to avoid reading the white line on 3 way intersections
      if (car_detected && !bulldozerMode) { 
        oldState = state;
        state = State::TRAFFIC;
        obstacleTime = millis();
        oldTimeDiff = millis() - oldTime;
      } else if (millis() > oldTime + turn_time) {
        state = State::FOLLOWING;
        gps_i = (gps_i + 1) % gps_size;
      }
      }

      break;
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
  error_history[error_hi] = error;
  error_hi = (error_hi + 1) % error_history_size;

  pos_history[pos_hi] = pos;
  pos_hi = (pos_hi + 1) % pos_history_size;

  cycle++;
  
}

void haltISR() {
  state = State::STOPPED;
}

void startISR() {
  state = State::FOLLOWING;
}

long pid_controller(long error) {
  /* The PID Controller that defines line following
   * Proportional: error used for e(t)
   * Integral: NO INTEGRAL COMPONENT (maybe add? I'm not the best with controllers, this project is my only experience)
   * Derivative: error - avg(error_history) is used as an estimate of the de(t)/dt.
   * NOTE: NONE OF THE VALUES ARE LIKELY OPTIMAL
   */
  //             Kp * e(t)  + Kd * de(t)/dt
  //                                VVV is this supposed to be error or steer.read()?
  return 5 * (error) / 8 - ((5  * (error - avg(error_history, error_history_size))) / 4);
}

long avg(long arr[], int arr_size) {
  long sum = 0;
  for (int i = 0; i <= arr_size; i++) { //size needs to be hardcoded
    sum += arr[i];
  }
  return sum / arr_size;
}

void stateLEDs(int state) {
  digitalWrite(RED, (state >> 2) & 0x01);
  digitalWrite(GREEN, (state >> 1) & 0x1);
  digitalWrite(YELLOW, (state) & 0x1);
}

int map_pos(int dist) {
  /* uses the map() function to translate the dist between the IR sensors
   * to a position on the servo, allowing us to compare apples to apples
   * for the controller.
   * 
   * DIST_MAX matches -turn_rad and DIST_MIN matches turn_rad
   * the map is a little backwards (i.e. DIFF_MIN is mapped to center + turn_rad)
   * because of the position of the servo. It just needs to flip it
   * 
   * IMPORTANT: Maps the DIST_CENTER to the center
   */
  int pos;
  if (dist < DIST_CENTER) {
    pos = map(dist, DIST_CENTER, DIST_MAX, center, center - left_turn_rad);
  } else {
    pos = map(dist, DIST_MIN, DIST_CENTER, center + left_turn_rad, center);
  }
  return pos;
}

int in_speed(int servo_pos) {
  const float wheel_radius = 10; //(mm)
  const float wheel_base = 86; //(mm), measured from front wheel axle to rear wheel axle
  const float wheel_dist = 40; //(mm), measure from inside of front right wheel to inside of front left wheel

  float turn_radius;
  turn_radius = wheel_base / tan(steering_angle(servo_pos)) - wheel_dist / 2;

  //ω * r = v <-- vehicle speed (const)
  //so ω_turn = ω_wheel * r_wheel / r_turn

  float ω_turn;
  ω_turn = SPEED * wheel_radius / turn_radius;
  return SPEED + (int)ω_turn;
}

int out_speed(int servo_pos) { //same as in_speed, but turn_radius is + wheel_dist / 2 instead of -
  const float wheel_radius = 10; //(mm)
  const float wheel_base = 86; //(mm), measured from front wheel axle to rear wheel axle
  const float wheel_dist = 40; //(mm), measure from inside of front right wheel to inside of front left wheel

  float turn_radius;
  turn_radius = wheel_base / tan(steering_angle(servo_pos)) + wheel_dist / 2;

  //ω * r = v <-- vehicle speed (const)
  //so ω_turn = ω_wheel * r_wheel / r_turn

  float ω_turn;
  ω_turn = SPEED * wheel_radius / turn_radius;
  Serial.println(ω_turn);
  return SPEED + (int)ω_turn;
}


float steering_angle(int servo_pos) { //returns IN RADIANS
  //THIS MIGHT NOT BE CORRECT, JUST LEAVING THIS HERE FOR NOW
  //71/4068 is an approximation of pi/180, accurate to 8 decimal places
  return (servo_pos - center) * 71 / 4068.0;
}
