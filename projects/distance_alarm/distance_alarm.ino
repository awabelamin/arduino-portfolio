/*
  Distance Alarm
  ---------------
  Parking-sensor style alarm using an HC-SR04 ultrasonic sensor.
    - Nothing in range  -> green LED, silent
    - Object in range   -> yellow LED, slow beeps
    - Object very close -> red LED, fast beeps
  The beep rate increases continuously as the object gets closer.
  Timing uses millis() so the sensor keeps reading while beeping.
*/

const uint8_t TRIG_PIN   = 9;
const uint8_t ECHO_PIN   = 10;
const uint8_t GREEN_LED  = 4;
const uint8_t YELLOW_LED = 5;
const uint8_t RED_LED    = 6;
const uint8_t BUZZER_PIN = 8;

const int SAFE_CM = 50;   // beyond this: all clear
const int WARN_CM = 20;   // closer than this: danger

unsigned long lastBeep = 0;

// Returns distance in cm, or -1 if no echo (nothing within ~5 m).
long readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return -1;
  return duration / 58;   // speed of sound: ~58 us per cm (round trip)
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  long d = readDistanceCm();
  bool inRange = (d > 0 && d <= SAFE_CM);

  digitalWrite(GREEN_LED,  !inRange);
  digitalWrite(YELLOW_LED, inRange && d > WARN_CM);
  digitalWrite(RED_LED,    inRange && d <= WARN_CM);

  if (inRange) {
    // Closer object -> shorter pause between beeps (60 ms .. 600 ms)
    long interval = map(constrain(d, 2, SAFE_CM), 2, SAFE_CM, 60, 600);
    if (millis() - lastBeep >= (unsigned long)interval) {
      tone(BUZZER_PIN, 2000, 40);
      lastBeep = millis();
    }
  }

  Serial.print("Distance: ");
  Serial.print(d);
  Serial.println(" cm");
  delay(50);
}
