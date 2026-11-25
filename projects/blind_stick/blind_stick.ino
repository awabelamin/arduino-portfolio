/*
  Smart Blind Stick
  ------------------
  An ultrasonic sensor mounted on a normal walking stick warns a
  visually impaired user about obstacles ahead.
    - Obstacle within ALERT_CM  -> short beeps + vibration pulses
    - The closer the obstacle, the faster the pulses
    - Very close (DANGER_CM)    -> continuous tone + vibration
*/

const uint8_t TRIG_PIN    = 9;
const uint8_t ECHO_PIN    = 10;
const uint8_t BUZZER_PIN  = 8;
const uint8_t VIBRATE_PIN = 6;   // vibration motor through an NPN transistor

const int ALERT_CM  = 100;
const int DANGER_CM = 25;

unsigned long lastPulse = 0;
bool pulseOn = false;

long readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return -1;
  return duration / 58;
}

void alertsOff() {
  noTone(BUZZER_PIN);
  digitalWrite(VIBRATE_PIN, LOW);
  pulseOn = false;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VIBRATE_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  long d = readDistanceCm();

  if (d < 0 || d > ALERT_CM) {
    alertsOff();
  } else if (d <= DANGER_CM) {
    // Very close: continuous warning
    tone(BUZZER_PIN, 2500);
    digitalWrite(VIBRATE_PIN, HIGH);
    pulseOn = true;
  } else {
    // Pulse period shrinks as the obstacle gets closer (100 .. 800 ms)
    unsigned long period = map(d, DANGER_CM, ALERT_CM, 100, 800);
    if (millis() - lastPulse >= period / 2) {
      lastPulse = millis();
      pulseOn = !pulseOn;
      if (pulseOn) {
        tone(BUZZER_PIN, 1800);
        digitalWrite(VIBRATE_PIN, HIGH);
      } else {
        noTone(BUZZER_PIN);
        digitalWrite(VIBRATE_PIN, LOW);
      }
    }
  }

  Serial.println(d);
  delay(30);
}
