/*
  Sequence Memory Game
  ---------------------
  A Simon-style memory game, inspired by the Human Benchmark "Sequence
  Memory" test. 4 LEDs, 4 buttons, 1 buzzer.

    1. The game shows a sequence of lights + tones.
    2. You repeat it with the buttons.
    3. Each round adds one more step to the sequence.
    4. One mistake (or 5 s without pressing) = game over.

  The high score is saved in EEPROM, so it survives power-off.
  Hold the RED button while powering on to reset the high score.
*/

#include <EEPROM.h>

const uint8_t LED_PINS[4]    = {2, 3, 4, 5};      // red, green, blue, yellow
const uint8_t BUTTON_PINS[4] = {6, 7, 8, 9};      // INPUT_PULLUP: pressed = LOW
const uint8_t BUZZER_PIN     = 10;
const int     TONES[4]       = {262, 330, 392, 523};  // C4, E4, G4, C5

const int MAX_LEVEL = 100;
const unsigned long INPUT_TIMEOUT_MS = 5000;
const int HIGH_SCORE_ADDR = 0;

uint8_t sequence[MAX_LEVEL];
int level = 0;

// ---------- Helpers ----------

void showPad(uint8_t i, unsigned int ms) {
  digitalWrite(LED_PINS[i], HIGH);
  tone(BUZZER_PIN, TONES[i]);
  delay(ms);
  digitalWrite(LED_PINS[i], LOW);
  noTone(BUZZER_PIN);
}

void allLeds(bool on) {
  for (uint8_t i = 0; i < 4; i++) digitalWrite(LED_PINS[i], on);
}

// Returns the pressed pad (0..3), or -1 on timeout.
// Lights the LED and plays its tone while the button is held.
int waitForButton(unsigned long timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    for (uint8_t i = 0; i < 4; i++) {
      if (digitalRead(BUTTON_PINS[i]) == LOW) {
        delay(20);                                   // debounce
        if (digitalRead(BUTTON_PINS[i]) != LOW) continue;
        digitalWrite(LED_PINS[i], HIGH);
        tone(BUZZER_PIN, TONES[i]);
        while (digitalRead(BUTTON_PINS[i]) == LOW) {} // wait for release
        delay(20);
        digitalWrite(LED_PINS[i], LOW);
        noTone(BUZZER_PIN);
        return i;
      }
    }
  }
  return -1;
}

void playSequence() {
  // Speeds up as the level rises: 500 ms per step down to 200 ms
  unsigned int stepMs = max(200, 500 - level * 20);
  for (int i = 0; i < level; i++) {
    showPad(sequence[i], stepMs);
    delay(stepMs / 2);
  }
}

bool playerRepeats() {
  for (int i = 0; i < level; i++) {
    int pressed = waitForButton(INPUT_TIMEOUT_MS);
    if (pressed != sequence[i]) return false;
  }
  return true;
}

// ---------- Effects ----------

void startAnimation() {
  for (uint8_t r = 0; r < 2; r++)
    for (uint8_t i = 0; i < 4; i++) showPad(i, 80);
  delay(500);
}

void levelUpEffect() {
  delay(300);
  allLeds(true);
  delay(120);
  allLeds(false);
  delay(500);
}

void gameOverEffect() {
  for (int f = 400; f >= 150; f -= 50) {
    tone(BUZZER_PIN, f, 120);
    allLeds(true);  delay(80);
    allLeds(false); delay(80);
  }
  noTone(BUZZER_PIN);
}

// Blink the score: every 10 levels = one long flash of all LEDs,
// remaining levels = short flashes of the green LED.
void showScore(int score) {
  delay(600);
  for (int i = 0; i < score / 10; i++) {
    allLeds(true);  delay(600);
    allLeds(false); delay(300);
  }
  for (int i = 0; i < score % 10; i++) {
    digitalWrite(LED_PINS[1], HIGH); delay(200);
    digitalWrite(LED_PINS[1], LOW);  delay(200);
  }
}

// ---------- Main ----------

void setup() {
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);

  if (digitalRead(BUTTON_PINS[0]) == LOW) {
    EEPROM.put(HIGH_SCORE_ADDR, 0);
    Serial.println("High score reset.");
  }
}

void loop() {
  int highScore;
  EEPROM.get(HIGH_SCORE_ADDR, highScore);
  if (highScore < 0 || highScore > MAX_LEVEL) highScore = 0;  // fresh EEPROM

  Serial.print("Press any button to start. High score: ");
  Serial.println(highScore);

  // Idle: slow chase until a button is pressed
  int pressed = -1;
  while (pressed < 0) {
    for (uint8_t i = 0; i < 4 && pressed < 0; i++) {
      digitalWrite(LED_PINS[i], HIGH);
      pressed = waitForButton(150);
      digitalWrite(LED_PINS[i], LOW);
    }
  }
  randomSeed(micros());       // human reaction time = good random seed

  startAnimation();
  level = 0;

  while (level < MAX_LEVEL) {
    sequence[level] = random(4);
    level++;
    playSequence();
    if (!playerRepeats()) break;
    levelUpEffect();
  }

  int score = level - 1;      // the last level was not completed
  gameOverEffect();
  Serial.print("Game over! Score: ");
  Serial.println(score);

  if (score > highScore) {
    EEPROM.put(HIGH_SCORE_ADDR, score);
    Serial.println("New high score!");
  }
  showScore(score);
  delay(1500);
}
