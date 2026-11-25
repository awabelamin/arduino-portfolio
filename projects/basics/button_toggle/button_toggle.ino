/*
  Button Toggle (with debouncing)
  Each press of the button toggles the LED on/off.
  The button uses the internal pull-up: pressed = LOW.
*/

const uint8_t BUTTON_PIN = 2;
const uint8_t LED_PIN    = 13;
const unsigned long DEBOUNCE_MS = 30;

bool ledOn = false;
bool lastReading = HIGH;
bool stableState = HIGH;
unsigned long lastChange = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastReading) {
    lastChange = millis();          // contact bounce: restart the timer
    lastReading = reading;
  }

  if (millis() - lastChange > DEBOUNCE_MS && reading != stableState) {
    stableState = reading;
    if (stableState == LOW) {       // a clean press
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn);
    }
  }
}
