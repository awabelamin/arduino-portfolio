/*
  Obstacle-Avoiding Car
  ----------------------
  4WD car with an HC-SR04 mounted on an SG90 servo ("head").
    1. Drive forward while the path is clear.
    2. When an obstacle is closer than STOP_CM: stop, reverse a little,
       look left and right, then turn toward the side with more space.

  Motors: left pair on L298N channel A, right pair on channel B.
  Note: the Servo library disables PWM on pins 9 and 10, so the
  motor enable pins use 5 and 6.
*/

#include <Servo.h>

// L298N
const uint8_t ENA = 5;   // left speed (PWM)
const uint8_t IN1 = 7;
const uint8_t IN2 = 8;
const uint8_t ENB = 6;   // right speed (PWM)
const uint8_t IN3 = 4;
const uint8_t IN4 = 2;

// Sensor head
const uint8_t SERVO_PIN = 3;
const uint8_t TRIG_PIN  = A0;
const uint8_t ECHO_PIN  = A1;

const int DRIVE_SPEED = 170;   // 0..255
const int TURN_SPEED  = 190;
const int STOP_CM     = 25;

const int LOOK_LEFT  = 160;
const int LOOK_AHEAD = 90;
const int LOOK_RIGHT = 20;

Servo head;

// speed: -255 (full reverse) .. 255 (full forward)
void driveSide(uint8_t en, uint8_t inA, uint8_t inB, int speed) {
  speed = constrain(speed, -255, 255);
  digitalWrite(inA, speed > 0);
  digitalWrite(inB, speed < 0);
  analogWrite(en, abs(speed));
}

void setMotors(int left, int right) {
  driveSide(ENA, IN1, IN2, left);
  driveSide(ENB, IN3, IN4, right);
}

void stopMotors() { setMotors(0, 0); }

long readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return 400;   // no echo = nothing close = clear
  return duration / 58;
}

long lookAt(int angle) {
  head.write(angle);
  delay(350);                      // let the servo arrive and settle
  return readDistanceCm();
}

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  head.attach(SERVO_PIN);
  head.write(LOOK_AHEAD);
  Serial.begin(9600);
  delay(1000);                     // time to put the car down
}

void loop() {
  long ahead = readDistanceCm();

  if (ahead > STOP_CM) {
    setMotors(DRIVE_SPEED, DRIVE_SPEED);
    delay(40);
    return;
  }

  // Obstacle: stop, back up, scan
  stopMotors();
  delay(150);
  setMotors(-DRIVE_SPEED, -DRIVE_SPEED);
  delay(300);
  stopMotors();

  long left  = lookAt(LOOK_LEFT);
  long right = lookAt(LOOK_RIGHT);
  head.write(LOOK_AHEAD);

  Serial.print("L="); Serial.print(left);
  Serial.print(" R="); Serial.println(right);

  if (left > right) {
    setMotors(-TURN_SPEED, TURN_SPEED);   // spin left
  } else {
    setMotors(TURN_SPEED, -TURN_SPEED);   // spin right
  }
  delay(400);
  stopMotors();
  delay(100);
}
