#include <SoftwareSerial.h>
#include <string.h>

#define MAX_STRING 256
#define TX_PIN  2
#define RX_PIN  3
 
SoftwareSerial bluetooth(RX_PIN, TX_PIN);

int motor1Pin1 = 4;
int motor1Pin2 = 5;
int motor1EnablePin = 6;
int motor2Pin1 = 8;
int motor2Pin2 = 9;
int motor2EnablePin = 10;
int motor3Pin1 = 11;
int motor3Pin2 = 12;
int Speed = 100;
int flag = 0;

char recv[MAX_STRING];
char state[MAX_STRING];
char tmp[MAX_STRING];

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor1EnablePin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor2EnablePin, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
}

void loop() {
  strcpy(state, "\0");
  strcpy(recv, "\0");
  if (bluetooth.available() > 0) {
    strcpy(recv, bluetooth.read());
    if (strlen(recv) > 1) {
      Speed = atoi(recv);
    } else {
      strcpy(state, recv);
    }
    if (strcmp(tmp, recv) != 0) {
      flag = 0;
    }
    strcpy(tmp, recv);
  }
  // a ~ FORWARD
  if (strcmp(state, "a") == 0) {
    forword();
    if (flag == 0) {
      Serial.println("Go Forward!");
      flag = 1;
    }
  }
  // c ~ TURN LEFT
  else if (strcmp(state, "c") == 0) {
    left();
    if (flag == 0) {
      Serial.println("Turn LEFT");
      flag = 1;
    }
  }
  // d ~ TURN RIGHT
  else if (strcmp(state, "d") == 0) {
    right();
    if (flag == 0) {
      Serial.println("Turn RIGHT");
      flag = 1;
    }
  }
  // b ~ BACK
  else if (strcmp(state, "b") == 0) {
    backword();
    if (flag == 0) {
      Serial.println("Reverse!");
      flag = 1;
    }
  }
  // e ~ stop_car
  else if (strcmp(state, "e") == 0) {
    stop_car();
    if (flag == 0) {
      Serial.println("Stop!");
      flag = 1;
    }
  }
  // ac ~ forward + left
  else if (strcmp(state, "ac") == 0) {
    forward_left();
    if (flag == 0) {
      Serial.println("Forward + Left!");
      flag = 1;
    }
  }
  // ad ~ forward + right
  else if (strcmp(state, "ad") == 0) {
    forward_right();
    if (flag == 0) {
      Serial.println("Forward + Left!");
      flag = 1;
    }
  }
  // bc ~ back + left
  else if (strcmp(state, "bc") == 0) {
    back_left();
    if (flag == 0) {
      Serial.println("Forward + Left!");
      flag = 1;
    }
  }
  // bd ~ back + right
  else if (strcmp(state, "bd") == 0) {
    back_right();
    if (flag == 0) {
      Serial.println("Forward + Left!");
      flag = 1;
    }
  }
  Serial.println(state);
}

void forword() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(motor1EnablePin, Speed);
  analogWrite(motor2EnablePin, Speed);
}

void backword() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(motor1EnablePin, Speed);
  analogWrite(motor2EnablePin, Speed);
}

void left() {
  digitalWrite(motor3Pin1, HIGH);
  digitalWrite(motor3Pin2, LOW);
}

void right() {
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
}

void forward_left() {
  digitalWrite(motor3Pin1, HIGH);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(motor1EnablePin, Speed);
  analogWrite(motor2EnablePin, Speed);
}

void forward_right() {
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(motor1EnablePin, Speed);
  analogWrite(motor2EnablePin, Speed);
}

void back_left() {
  digitalWrite(motor3Pin1, HIGH);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(motor1EnablePin, Speed);
  analogWrite(motor2EnablePin, Speed);
}

void back_right() {
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(motor1EnablePin, Speed);
  analogWrite(motor2EnablePin, Speed);
}

void stop_car() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(motor1EnablePin, Speed);
  analogWrite(motor2EnablePin, Speed);
}
