////////////////////////////////////////////////////////////////////////////////
// Filename: Junior_bot.ino
// Author: SafeTown (2023-2025)
// Purpose: Latest version of functional software for the junior robot.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Setup
////////////////////////////////////////////////////////////////////////////////
// Library Declarations
#include <Servo.h>
#include <Motor.h>
// #include <DisplayOurValues.h> // used for display
#include <SafeTownDisplay.h>

// Line-following constants
#define OUTSIDE_MAX 900
#define INSIDE_MAX  850
#define ASYMPTOTE   85
#define DIFF_MAX    850
#define DIFF_MIN    -650
#define SUM_MIN     750
#define SUM_MAX     1200
#define FRONT_THRES 400
#define DOWN_THRES  300

// Pin I/O definitions
#define STOP     17
#define START    16
#define DISP_SDA 4
#define DISP_SCL 5
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
#define TURN_L   2
#define TURN_R   3
#define IR_F     28
#define IR_I     27
#define IR_O     26
#define IR_D     29

// Steering variables
Servo steer;
long pos;

// Display variables
// DisplayOurValues display = DisplayOurValues(); // initialize display
SafeTownDisplay display = SafeTownDisplay(); // initialize display

// Encoder variables
bool encoderChange = false; // track encoder changes

// Data collection variables
// int timeOffset = 0;
// int samplePointer = 0;
// bool collectData = false;
// String dataOutput = "";
// const int NUM_SAMPLES = 100;
// int times[NUM_SAMPLES] = {};
// int downSamples[NUM_SAMPLES] = {};
// int frontSamples[NUM_SAMPLES] = {};
// int innerLeftSamples[NUM_SAMPLES] = {};
// int outerLeftSamples[NUM_SAMPLES] = {};
// bool printData = false;
// int sampleTargetTime = 0;

bool bulldozerMode = true;
int loopCounter = 0;

// Data collection parameters
// int sampleRate = 20; // Units: samples/s
// int sampleInterval = 1000/sampleRate; // Units: ms/sample

// Encoder left turn
void ENC_B_GO() {
  if (encoderChange) {
    // display.decrementMenuIndex();
    display.encoderLeft();
  }
  encoderChange = !encoderChange;
}

// Encoder right turn
void ENC_A_GO() {
  if (encoderChange) {
    // display.incrementMenuIndex();
    display.encoderRight();
  }
  encoderChange = !encoderChange;
}

// Encoder press
void ENC_S_GO() {
  encoderChange = false;
  display.encoderPress();
  // if (display.getMenuType() == 10) { // 10 is the temporary hardcoded number for SAVE
  //   printData = true;
  // }
  // display.goToMenu();
  // if (display.getMenuType() == 10) { // 10 is the temporary hardcoded number for SAVE
  //   timeOffset = int(millis());
  //   samplePointer = 0;
  //   sampleTargetTime = 0;
  //   collectData = true;
  // }
}

//XY where X stands for direction going to (straight, left, or right) and Y stands for direction coming from (straight, left, or right)
//X from Y
//ex. if we are going straight through the intersection, but approaching it from a right hand turn, it would be SR or Straight from Right
/*enum Intersection_Directions {
  LL,
  LS,
  LR,
  SL = -31,
  SS = -10,
  SR =  70,
  RL = -26,
  RS,
  RR
}*/

// Navigation variables
int gps_i = 0;
const int gps_size = 4;
//0 is straight, 1 is left, 2 is right
const int gps[] = {0, 1, 2, 0};

// Obstacle detection variables
long cycle = 0;
long oldTime = 0, obstacleTime = 0;
long led_time = 0;
long oldTimeDiff = 0;

// Turning calibration variables
const int turn_rad = 50; //50 for MARS
const int center = 70; //70 is center for MARS

// Line-following variables
int inside, outside;
long error;
long sum, difference;
long error_history[256] = {0};
int error_hi = 0;
const int eh_size = 256;

// Motor variables
Motor left_motor(LEFT_A, LEFT_B);
Motor right_motor(RIGHT_A, RIGHT_B);
const int speed = 128;
bool right_brake = true, left_brake = true;
bool left = false, right = false;

// FSM states
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

void setup() {
  pinMode(IR_I, INPUT);
  pinMode(IR_O, INPUT);
  pinMode(IR_D, INPUT);
  pinMode(IR_F, INPUT);

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

  // Display setup
  display.setup();
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  // Encoder setup
  pinMode(ENC_B, INPUT);
  pinMode(ENC_A, INPUT);
  pinMode(ENC_S, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_B), ENC_B_GO, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_A), ENC_A_GO, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_S), ENC_S_GO, RISING);

  steer.attach(SERVO);

  left_motor.begin();
  right_motor.begin();
}

void loop() {
  loopCounter++;
  inside = analogRead(IR_I);
  outside = analogRead(IR_O);
  difference = outside - inside;
  sum = outside + inside;

  // Write to OLED display
  if (loopCounter >= 10000) {
    // display.setUpdateScreen(true);
    // display.displayMenu();
    loopCounter = 0;
  }

  display.displayMenu(loopCounter == 0);

  // Collect IR data
  // int time = int(millis()) - timeOffset;
  // if (collectData && time >= sampleTargetTime + sampleInterval) {
  //   int down = analogRead(IR_D);
  //   int front = analogRead(IR_F);
  //   int innerLeft = analogRead(IR_I);
  //   int outerLeft = analogRead(IR_O);
  //   downSamples[samplePointer] = down;
  //   frontSamples[samplePointer] = front;
  //   innerLeftSamples[samplePointer] = innerLeft;
  //   outerLeftSamples[samplePointer] = outerLeft;
  //   times[samplePointer] = time;
  //   Serial.println("Collecting data: " + String(samplePointer+1) + "/" + String(NUM_SAMPLES));
  //   samplePointer++;
  //   sampleTargetTime += sampleInterval;
  //   if (samplePointer >= NUM_SAMPLES) {
  //     collectData = false;
  //   }
  // }

  // use brake light to indicate data collection mode
  // if (collectData) {
  //   digitalWrite(BRAKE_LIGHT, HIGH);
  // }

  // if (printData) {
  //   Serial.println("-----BEGIN DATA-----");
  //   Serial.println("DATA TYPE: IR_Sensors");
  //   Serial.println("Sample,Millis,Down,Front,InnerLeft,OuterLeft");
  //   for (int i = 0; i < NUM_SAMPLES; i++) {
  //     Serial.print(i+1);
  //     Serial.print(",");
  //     Serial.print(times[i]);
  //     Serial.print(",");
  //     Serial.print(downSamples[i]);
  //     Serial.print(",");
  //     Serial.print(frontSamples[i]);
  //     Serial.print(",");
  //     Serial.print(innerLeftSamples[i]);
  //     Serial.print(",");
  //     Serial.println(outerLeftSamples[i]);
  //   }
  //   Serial.println("-----END DATA-----");
  //   printData = false;
  // }

  stateLEDs(state);
  switch (state) {
    case State::FOLLOWING:
      right_brake = false;
      left_brake = false;
      left = false;
      right = false;
      //maps the correction factor to the turn radius      
      error = map_pos(difference) - center;

      pos = center + pid_controller(error);
      steer.write(pos);
      
      //checks if we have lost the line
      //sum is used b/c it tells us when we leave the range of acceptable values
      //NOTE: difference will be >> 0 or << 0, never near 0, so this simple check is sufficient
      if (sum > SUM_MAX) {
        //if diff > asymptote, we are too close
        if (difference > ASYMPTOTE) {
          state = State::CLOSE;
        //and if it diff < asymptote negative we are too far
        } else {
          state = State::FAR;
        }
      } else if (analogRead(IR_F) < FRONT_THRES) { 
        if (!bulldozerMode) {
          oldState = state;
          state = State::TRAFFIC;
          obstacleTime = millis();
        }
      } else if (analogRead(IR_D) < DOWN_THRES) {
        oldTime = millis();
        state = State::INTERSECTION_WAIT;
      }

      break;
    case State::FAR:
      right_brake = false;
      left_brake = false;
      left = false;
      right = false;
      //turns as HARD as possible toward the line
      error = turn_rad;
      pos = center + turn_rad;
      steer.write(pos);

      //checks if we are back in range to continue following the line normally
      if (sum < SUM_MIN) {
        state = State::FOLLOWING;
      } else if (analogRead(IR_F) < FRONT_THRES) { 
        if (!bulldozerMode) {
          oldState = state;
          state = State::TRAFFIC;
          obstacleTime = millis();
        }
      } else if (analogRead(IR_D) < DOWN_THRES) {
        oldTime = millis();
        state = State::INTERSECTION_WAIT;
      }
      break;
    case State::CLOSE:
      right_brake = false;
      left_brake = false;
      left = false;
      right = false;
      //turns as HARD as possible away from the line
      error = -turn_rad;
      pos = center - turn_rad;
      steer.write(pos);

      //checks if we are back in range to continue following the line normally
      if (sum < SUM_MIN) {
        state = State::FOLLOWING;
      } else if (analogRead(IR_F) < FRONT_THRES) { 
        if (!bulldozerMode) {
          oldState = state;
          state = State::TRAFFIC;
          obstacleTime = millis();
        }
      }
      break;
    case State::STOPPED:
      //stop the robot
      error = 0;
      right_brake = true;
      left_brake = true;
      left = false;
      right = false;
      steer.write(center);
      digitalWrite(BUZZER, LOW);
      gps_i = 0; //resets the path, might not be desired functionality.
                 //treats STOPPED as a stop, instead of a pause.
                 //remove these lines if you want a pause.
      break;
    case State::INTERSECTION_WAIT:
      //WAIT at intersection
      right_brake = true;
      left_brake = true;
      left = gps[gps_i] == 1;
      right = gps[gps_i] == 2;
      if (analogRead(IR_F) < FRONT_THRES) { //if intersection is NOT clear, wait until it is
        if (!bulldozerMode) {
          oldState = state;
          state = State::TRAFFIC;
          obstacleTime = millis();
          oldTimeDiff = millis() - oldTime;
        }
      } else if (millis() > oldTime + 1000) { //otherwise, wait 1 second and then GO GO GO!
        oldTime = millis();
        switch (gps[gps_i]) {
          case 2:
            state = State::INTERSECTION_NAV;
            break;
          default:
            state = State::INTERSECTION_FOR;
            break;
        }
        
      }
      break;
    case INTERSECTION_FOR:
      right_brake = false;
      left_brake = false;
      //SET oldTime = millis(); when entering this state
      //go forward to line up the back wheels with the stop line before turning
      error = 3 * avg(error_history) / 4; // keep same error
      pos = center + error;
      steer.write(pos);
      if (analogRead(IR_F) < FRONT_THRES) { 
        if (!bulldozerMode) {
          oldState = state;
          state = State::TRAFFIC;
          obstacleTime = millis();
          oldTimeDiff = millis() - oldTime;
        }
      } else if (millis() > oldTime + (520 * 128 / speed)) {
        oldTime = millis();
        state = State::INTERSECTION_NAV;
      }
      break;
    
    case INTERSECTION_NAV:
      left_brake = false;
      right_brake = false;
      left = gps[gps_i] == 1;
      right = gps[gps_i] == 2;
      /* NOTE: All of these cases include an -avg(error_history)
       *       This predicts the angle we have come from to adjust for entering intersections after curves
       * NOT CURRENTLY, I found it to be unreliable, at least straight as -avg(eh), no adjustments. So some work needs to be done here
      */
      switch (gps[gps_i]) {
        case 0: //straight
          //                 VVVV this adjustment factor is TEMPORARY - it should be replaced with the negative average of the position history
          steer.write(center - 5);
          break;
        case 1: //left
          //THIS IS WRONG, USE AVG OF HISTORY INSTEAD
          //int theta;
          //theta = center - prev_pos; //angle of servo is inverse angle of robot b/c it's trying to correct

          //if under turning (going off on the right of the line), increase the number
          //if over  turning (going off on the left  of the line), decrease the number
          //                   VV this is the number, the comments above ignore the negative 
          steer.write(center - 40);
          break;
        case 2: //right
          //anchors right wheel
          steer.write(center + 85);
          break;
        default:
          break;
      }
      //makes us wait 0.25 seconds before being able to switch to avoid reading the white line on 3 way intersections
      if (analogRead(IR_F) < FRONT_THRES) { 
        if (!bulldozerMode) {
          oldState = state;
          state = State::TRAFFIC;
          obstacleTime = millis();
          oldTimeDiff = millis() - oldTime;
        }
      } else if (millis() > oldTime + 200 && sum < SUM_MIN) {
        state = State::FOLLOWING;
        gps_i = (gps_i + 1) % gps_size;
      }

      break;

    case TRAFFIC:
      //SET obstactleTime = millis(); IN THE STATE JUST BEFORE ENTERING, like in FOLLOWING
      right_brake = true;
      left_brake = true;
      left = left; //keep these the same
      right = right; //left here for symbolic purposes, the compiler gets rid of these lines.
      //if the road is clear, wait 0.75s before going forward
      if (millis() > obstacleTime + 750) {
        state = oldState;
        oldTime = millis() - oldTimeDiff;
      } else if (!(analogRead(IR_F) > FRONT_THRES)) {
        obstacleTime = millis();
      }
      break;
    default: 
      //IF THE ROBOT IS HERE, SOMETHING IS VERY WRONG
      //in any other state, stop the robot and turn on the buzzer
      right_brake = true;
      left = true;
      right = true;
      steer.write(center);
      analogWrite(BUZZER, 30);
      break;
  }



  //THIS IS THE CODE FOR THE DIFFERENTIAL DRIVE - IT DOES NOT WORK, please help
  /*
  //determines which is the inside and the outside and gives the corresponding speed
  if (!brake && steer.read() > center) {
    left.forward(in_speed(steer.read()));
    right.forward(out_speed(steer.read()));
  } else if (!brake) {
    left.forward(out_speed(steer.read()));
    right.forward(in_speed(steer.read()));
  } else {
    left.brake();
    right.brake();
  }*/

  if(right_brake) {
    right_motor.brake();
  } else {
    right_motor.forward(speed);
  }

  if(left_brake) {
    left_motor.brake();
  } else {
    left_motor.forward(speed);
  }



  //LED Control

  //brake lights are on if we are braking
  // if (!collectData) {
    if(right_brake && left_brake) {
      digitalWrite(BRAKE_LIGHT, HIGH);
    } else {
      digitalWrite(BRAKE_LIGHT, LOW);
    }
  // }

  //left lights blink on and off with a period of 1 second, otherwise are off
  if (left && millis() % 1000 > 500) {
    digitalWrite(TURN_L, HIGH);
  } else {
    digitalWrite(TURN_L, LOW);
  }

  //right lights blink on and off with a period of 1 second, otherwise are off
  if (right && millis() % 1000 > 500) {
    digitalWrite(TURN_R, HIGH);
  } else {
    digitalWrite(TURN_R, LOW);
  }

  //keeps a history of error for 2 purposes:
  //1.) D and I components of the PID controller
  //2.) allows us to adjust our turn angle at an
  //    intersection based on the angle of the robot
  error_history[error_hi] = error;
  error_hi = (error_hi + 1) % eh_size;
  cycle ++;
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
  return 5 * (error) / 8 - ((5  * (error - avg(error_history))) / 4);
}

long avg(long arr[]) {
  long sum = 0;
  for (int i = 0; i <= eh_size; i++) {
    sum += arr[i];
  }
  return sum / eh_size;
}

void stateLEDs(int state) {
  digitalWrite(RED, (state >> 2) & 0x1);
  digitalWrite(GREEN, (state >> 1) & 0x1);
  digitalWrite(YELLOW, (state) & 0x1);
}

int map_pos(int diff) {
  /* uses the map() function to translate the difference between the IR sensors
   * to a position on the servo, allowing us to compare apples to apples
   * for the controller.
   * 
   * DIFF_MAX matches -turn_rad and DIFF_MIN matches turn_rad
   * the map is a little backwards (i.e. DIFF_MIN is mapped to center + turn_rad)
   * because of the position of the servo. It just needs to flip it
   * 
   * IMPORTANT: Maps the ASYMPTOTE to the center
   */
  int pos;
  if (diff > ASYMPTOTE) {
    pos = map(diff, ASYMPTOTE, DIFF_MAX, center, center - turn_rad);
  } else {
    pos = map(diff, DIFF_MIN, ASYMPTOTE, center + turn_rad, center);
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
  ω_turn = speed * wheel_radius / turn_radius;
  return speed + (int)ω_turn;
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
  ω_turn = speed * wheel_radius / turn_radius;
  Serial.println(ω_turn);
  return speed + (int)ω_turn;
}


float steering_angle(int servo_pos) { //returns IN RADIANS
  //THIS MIGHT NOT BE CORRECT, JUST LEAVING THIS HERE FOR NOW
  //71/4068 is an approximation of pi/180, accurate to 8 decimal places
  return (servo_pos - center) * 71 / 4068.0;
}
