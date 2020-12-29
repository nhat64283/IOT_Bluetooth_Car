#include <SoftwareSerial.h>

// set using pin
#define TX_PIN  2
#define RX_PIN  3
#define MOTOR12PIN1 4
#define MOTOR12PIN2 5
#define MOTOR12ENABLEPIN 6
#define MOTOR3PIN1 11
#define MOTOR3PIN2 12
// set all state's value
#define NONE 0
#define FORWARD_HOLD 1
#define BACK_HOLD 2
#define LEFT_HOLD 3
#define RIGHT_HOLD 4
#define STOP_HOLD 5
#define FORWARD_RELEASE 6
#define BACK_RELEASE 7
#define LEFT_RELEASE 8
#define RIGHT_RELEASE 9
#define STOP_RELEASE 10

SoftwareSerial bluetooth(RX_PIN, TX_PIN);

int Speed = 100;
int state, state_forward_back, state_left_right, state_stop,
    value1pin1, value1pin2, value2pin1, value2pin2;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  //initialize motor's value
  set_low_low(&value1pin1, &value1pin2);
  set_low_low(&value2pin1, &value2pin2);

  //initialize all state
  state = NONE;
  state_stop = NONE;
  state_forward_back = NONE;
  state_left_right = NONE;

  //set pin's mode = output
  pinMode(MOTOR12PIN1, OUTPUT);
  pinMode(MOTOR12PIN2, OUTPUT);
  pinMode(MOTOR12ENABLEPIN, OUTPUT);
  pinMode(MOTOR3PIN2, OUTPUT);
  pinMode(MOTOR3PIN2, OUTPUT);
}

void loop() {
  state = NONE;
  if (bluetooth.available() > 0) state = bluetooth.read();

  // if there are some chage
  if (state != NONE) {
    switch (state) {
      case FORWARD_HOLD:
        // if back button is holding then stop forward and back
        if (state_forward_back == BACK_HOLD) {
          // save the state that both forward and back is holding
          state_forward_back = FORWARD_HOLD + BACK_HOLD;
          // if stop button is holding then keep stop
          if (state_stop == STOP_HOLD) break;
          set_low_low(&value1pin1, &value1pin2);
        } else {
          state_forward_back = FORWARD_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_high_low(&value1pin1, &value1pin2);
        }
        break;
      case BACK_HOLD:
        if (state_forward_back == FORWARD_HOLD) {
          state_forward_back = FORWARD_HOLD + BACK_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_low_low(&value1pin1, &value1pin2);
        } else {
          state_forward_back = BACK_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_high_low(&value1pin2, &value1pin1);
        }
        break;
      case LEFT_HOLD:
        if (state_left_right == RIGHT_HOLD) {
          state_left_right = LEFT_HOLD + RIGHT_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_low_low(&value2pin1, &value2pin2);
        } else {
          state_left_right = LEFT_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_high_low(&value2pin1, &value2pin2);
        }
        break;
      case RIGHT_HOLD:
        if (state_left_right == LEFT_HOLD) {
          state_left_right = LEFT_HOLD + RIGHT_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_low_low(&value2pin1, &value2pin2);
        } else {
          state_left_right = RIGHT_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_high_low(&value2pin2, &value2pin1);
        }
        break;
      case STOP_HOLD:
        // set all motor stop
        set_low_low(&value1pin1, &value1pin2);
        set_low_low(&value2pin1, &value2pin2);
        state_stop = STOP_HOLD;
        break;
      case FORWARD_RELEASE:
        // if previous forward button is holding then stop forward
        if (state_forward_back == FORWARD_HOLD) {
          state_forward_back = NONE;
          if (state_stop == STOP_HOLD) break;
          set_low_low(&value1pin1, &value1pin2);
        }
        // if both forward and back is holding then motor go back
        else if (state_forward_back == FORWARD_HOLD + BACK_HOLD) {
          state_left_right = BACK_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_high_low(&value1pin2, &value1pin1);
        }
        break;
      case BACK_RELEASE:
        if (state_forward_back == BACK_HOLD) {
          state_forward_back = NONE;
          if (state_stop == STOP_HOLD) break;
          set_low_low(&value1pin1, &value1pin2);
        } else if (state_forward_back == FORWARD_HOLD + BACK_HOLD) {
          state_left_right = FORWARD_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_high_low(&value1pin1, &value1pin2);
        }
        break;
      case LEFT_RELEASE:
        if (state_left_right == LEFT_HOLD) {
          state_left_right = NONE;
          if (state_stop == STOP_HOLD) break;
          set_low_low(&value2pin1, &value2pin2);
        } else if (state_left_right == LEFT_HOLD + RIGHT_HOLD) {
          state_left_right = RIGHT_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_high_low(&value2pin2, &value2pin1);
        }
        break;
      case RIGHT_RELEASE:
        if (state_left_right == RIGHT_HOLD) {
          state_left_right = NONE;
          if (state_stop == STOP_HOLD) break;
          set_low_low(&value2pin1, &value2pin2);
        } else if (state_left_right == LEFT_HOLD + RIGHT_HOLD) {
          state_left_right = LEFT_HOLD;
          if (state_stop == STOP_HOLD) break;
          set_high_low(&value2pin1, &value2pin2);
        }
        break;
      case STOP_RELEASE:
        if (state_forward_back == FORWARD_HOLD) {
          set_high_low(&value1pin1, &value1pin2);
        }
        if (state_forward_back == BACK_HOLD) {
          set_high_low(&value1pin2, &value1pin1);
        }
        if (state_left_right == LEFT_HOLD) {
          set_high_low(&value2pin1, &value2pin2);
        }
        if (state_left_right == RIGHT_HOLD) {
          set_high_low(&value2pin2, &value2pin1);
        }
        state_stop = STOP_RELEASE;
        break;
      default:
        Speed = state;
        break;
    }
    Serial.println(state);
  }
  digitalWrite(MOTOR12PIN1, value1pin1);
  digitalWrite(MOTOR12PIN2, value1pin2);
  digitalWrite(MOTOR3PIN1, value2pin1);
  digitalWrite(MOTOR3PIN2, value2pin2);
  analogWrite(MOTOR12ENABLEPIN, Speed);
}

void set_high_low(int *pin1, int *pin2) {
  *pin1 = HIGH;
  *pin2 = LOW;
}

void set_low_low(int *pin1, int *pin2) {
  *pin1 = LOW;
  *pin2 = LOW;
}
