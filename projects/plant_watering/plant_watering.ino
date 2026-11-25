/*
  Automatic Plant Watering System
  --------------------------------
  Reads a soil-moisture sensor and runs a small water pump (via a relay)
  only when the soil is dry.

  State machine:
    MONITORING -> soil dry?                    -> WATERING
    WATERING   -> soil wet OR max time reached -> SOAKING
    SOAKING    -> wait for water to spread     -> MONITORING

  Two thresholds (hysteresis) stop the pump from rapidly switching on/off,
  and a max pump time protects against a dry tank or a broken sensor.
*/

const uint8_t SOIL_PIN   = A0;
const uint8_t RELAY_PIN  = 7;
const uint8_t STATUS_LED = 13;

// Analog readings: higher = drier for most soil sensors. Calibrate yours:
// read the Serial Monitor with the probe in dry soil and in wet soil.
const int DRY_THRESHOLD = 600;   // start watering above this
const int WET_THRESHOLD = 450;   // stop watering below this

const unsigned long MAX_PUMP_MS  = 5000UL;    // safety limit per watering
const unsigned long SOAK_WAIT_MS = 30000UL;   // let water spread before re-checking

const bool RELAY_ACTIVE_LOW = true;           // most relay modules switch on with LOW

enum State { MONITORING, WATERING, SOAKING };
State state = MONITORING;
unsigned long stateStart = 0;

void pump(bool on) {
  digitalWrite(RELAY_PIN, (on ^ RELAY_ACTIVE_LOW) ? HIGH : LOW);
  digitalWrite(STATUS_LED, on);
}

int readSoil() {
  long sum = 0;
  for (int i = 0; i < 10; i++) {   // average out noise
    sum += analogRead(SOIL_PIN);
    delay(5);
  }
  return sum / 10;
}

void enterState(State s) {
  state = s;
  stateStart = millis();
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  pump(false);
  Serial.begin(9600);
}

void loop() {
  int soil = readSoil();
  unsigned long elapsed = millis() - stateStart;

  switch (state) {
    case MONITORING:
      if (soil > DRY_THRESHOLD) {
        pump(true);
        enterState(WATERING);
      }
      break;

    case WATERING:
      if (soil < WET_THRESHOLD || elapsed >= MAX_PUMP_MS) {
        pump(false);
        enterState(SOAKING);
      }
      break;

    case SOAKING:
      if (elapsed >= SOAK_WAIT_MS) {
        enterState(MONITORING);
      }
      break;
  }

  Serial.print("Soil: ");
  Serial.print(soil);
  Serial.print("  State: ");
  Serial.println(state == MONITORING ? "MONITORING" : state == WATERING ? "WATERING" : "SOAKING");
  delay(200);
}
