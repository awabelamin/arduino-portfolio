/*
  Traffic Light
  Cycles red -> red+yellow -> green -> yellow -> red,
  like a real traffic light.
*/

const uint8_t RED_PIN    = 4;
const uint8_t YELLOW_PIN = 3;
const uint8_t GREEN_PIN  = 2;

void setLights(bool r, bool y, bool g, unsigned long ms) {
  digitalWrite(RED_PIN, r);
  digitalWrite(YELLOW_PIN, y);
  digitalWrite(GREEN_PIN, g);
  delay(ms);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
  setLights(1, 0, 0, 4000);   // stop
  setLights(1, 1, 0, 1000);   // get ready
  setLights(0, 0, 1, 4000);   // go
  setLights(0, 1, 0, 1500);   // slow down
}
