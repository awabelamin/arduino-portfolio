/*
  Lemon Color Sorter (green vs. yellow)
  --------------------------------------
  A lemon rolls down a ramp and stops under a TCS3200 color sensor.
  The Arduino measures its red / green / blue light, decides whether it
  is YELLOW (ripe) or GREEN (unripe), and turns a servo flap so the
  lemon drops into the right box.

  TCS3200 basics:
    - S0/S1 set the output frequency scale (we use 20%).
    - S2/S3 pick the color filter (red, green, blue).
    - OUT is a square wave; pulseIn() gives its period.
      SHORTER period = MORE light of that color.

  Why red vs. green works:
    Yellow = red + green light, so a yellow lemon reflects a lot of RED.
    A green lemon reflects much less red than green.
    -> ratio = redPeriod / greenPeriod
       yellow lemon: ratio ~1.0 or lower
       green lemon : ratio clearly > 1

  Use CALIBRATION_MODE to print raw readings and tune the constants.
*/

#include <Servo.h>

const uint8_t S0 = 4, S1 = 5, S2 = 6, S3 = 7;
const uint8_t SENSOR_OUT = 8;
const uint8_t SERVO_PIN  = 9;

const bool CALIBRATION_MODE = false;

// --- Tune these from the Serial Monitor in CALIBRATION_MODE ---
const unsigned long PRESENCE_MAX = 250;  // red+green+blue period below this = lemon present
const float YELLOW_MAX_RATIO     = 1.15; // red/green ratio at or below this = yellow

const int FLAP_CENTER = 90;
const int FLAP_YELLOW = 40;
const int FLAP_GREEN  = 140;

Servo flap;
unsigned int yellowCount = 0, greenCount = 0;

unsigned long readPeriod(bool s2, bool s3) {
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
  delay(5);                                         // let the filter settle
  unsigned long p = pulseIn(SENSOR_OUT, LOW, 50000UL);
  return p == 0 ? 50000UL : p;                      // no signal = very dark
}

// Average a few readings for stability.
void readRGB(unsigned long &r, unsigned long &g, unsigned long &b) {
  r = g = b = 0;
  for (int i = 0; i < 5; i++) {
    r += readPeriod(LOW,  LOW);    // red filter
    g += readPeriod(HIGH, HIGH);   // green filter
    b += readPeriod(LOW,  HIGH);   // blue filter
  }
  r /= 5; g /= 5; b /= 5;
}

void sortTo(int angle) {
  flap.write(angle);
  delay(800);          // lemon falls through
  flap.write(FLAP_CENTER);
  delay(400);
}

void setup() {
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(SENSOR_OUT, INPUT);

  digitalWrite(S0, HIGH);   // 20% frequency scaling
  digitalWrite(S1, LOW);

  flap.attach(SERVO_PIN);
  flap.write(FLAP_CENTER);
  Serial.begin(9600);
  Serial.println("Lemon sorter ready");
}

void loop() {
  unsigned long r, g, b;
  readRGB(r, g, b);

  if (CALIBRATION_MODE) {
    Serial.print("R="); Serial.print(r);
    Serial.print(" G="); Serial.print(g);
    Serial.print(" B="); Serial.print(b);
    Serial.print(" sum="); Serial.print(r + g + b);
    Serial.print(" R/G="); Serial.println((float)r / g, 2);
    delay(300);
    return;
  }

  if (r + g + b > PRESENCE_MAX) {   // too dark/empty: no lemon in place
    delay(100);
    return;
  }

  float ratio = (float)r / g;
  if (ratio <= YELLOW_MAX_RATIO) {
    yellowCount++;
    Serial.print("YELLOW  ratio=");
    Serial.println(ratio, 2);
    sortTo(FLAP_YELLOW);
  } else {
    greenCount++;
    Serial.print("GREEN   ratio=");
    Serial.println(ratio, 2);
    sortTo(FLAP_GREEN);
  }

  Serial.print("Totals -> yellow: "); Serial.print(yellowCount);
  Serial.print("  green: ");          Serial.println(greenCount);
}
