/*
  Line Follower Robot
  --------------------
  Two IR reflectance sensors straddle a black line on a light floor.
    - Both sensors on the floor (white) -> go straight
    - Left sensor sees the line         -> turn left
    - Right sensor sees the line        -> turn right
    - Both see the line (junction/end)  -> stop

  Many IR modules output LOW on white and HIGH on black. If yours is the
  opposite, change LINE_DETECTED to LOW.
*/

const uint8_t ENA = 5;   // left speed (PWM)
const uint8_t IN1 = 7;
const uint8_t IN2 = 8;
const uint8_t ENB = 6;   // right speed (PWM)
const uint8_t IN3 = 4;
const uint8_t IN4 = 2;

const uint8_t LEFT_SENSOR  = A0;
const uint8_t RIGHT_SENSOR = A1;
const int LINE_DETECTED = HIGH;

const int BASE_SPEED = 150;
const int TURN_FAST  = 170;
const int TURN_SLOW  = -60;   // inner wheel slightly reversed for sharper curves

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

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  delay(1000);
}

void loop() {
  bool leftOnLine  = digitalRead(LEFT_SENSOR)  == LINE_DETECTED;
  bool rightOnLine = digitalRead(RIGHT_SENSOR) == LINE_DETECTED;

  if (!leftOnLine && !rightOnLine) {
    setMotors(BASE_SPEED, BASE_SPEED);          // centered: straight
  } else if (leftOnLine && !rightOnLine) {
    setMotors(TURN_SLOW, TURN_FAST);            // line drifted left: turn left
  } else if (!leftOnLine && rightOnLine) {
    setMotors(TURN_FAST, TURN_SLOW);            // line drifted right: turn right
  } else {
    setMotors(0, 0);                            // both on line: stop
  }
}
