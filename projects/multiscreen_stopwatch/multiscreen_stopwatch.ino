/*
  Multi-Screen Clock & Stopwatch
  -------------------------------
  A dual-function timekeeping system on one 16x2 LCD with three screens:

    [CLOCK]      12:34:56        set hours/minutes with the buttons
    [STOPWATCH]  00:01:23.45     start / stop, lap, reset
    [LAPS]       Lap 1..5        scroll through recorded laps

  Buttons (INPUT_PULLUP, pressed = LOW):
    MODE  -> next screen
    A     -> CLOCK: hour +1      STOPWATCH: start/stop    LAPS: next lap
    B     -> CLOCK: minute +1    STOPWATCH: lap (running) / reset (stopped)

  Everything is timed with millis(), so the clock keeps running while the
  stopwatch is used, and the display never blocks the buttons.
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);   // RS, E, D4, D5, D6, D7

const uint8_t BTN_MODE = 8;
const uint8_t BTN_A    = 9;
const uint8_t BTN_B    = 10;
const uint8_t BUZZER   = 7;

enum Screen { CLOCK, STOPWATCH, LAPS };
Screen screen = CLOCK;

// ---- Clock ----
unsigned long clockBaseMs = 0;     // millis() when the clock read 00:00:00
unsigned long clockOffsetS = 12UL * 3600;   // starts at 12:00:00

// ---- Stopwatch ----
bool running = false;
unsigned long swStartMs = 0;       // millis() when the current run started
unsigned long swAccumMs = 0;       // time from previous runs

const uint8_t MAX_LAPS = 5;
unsigned long laps[MAX_LAPS];
uint8_t lapCount = 0;
uint8_t lapView = 0;

// ---- Buttons (debounced, fire once per press) ----
struct Button {
  uint8_t pin;
  bool stable;
  bool lastReading;
  unsigned long lastChange;
};
Button buttons[3] = {
  {BTN_MODE, HIGH, HIGH, 0},
  {BTN_A,    HIGH, HIGH, 0},
  {BTN_B,    HIGH, HIGH, 0},
};

bool pressed(Button &b) {
  bool reading = digitalRead(b.pin);
  if (reading != b.lastReading) {
    b.lastChange = millis();
    b.lastReading = reading;
  }
  if (millis() - b.lastChange > 25 && reading != b.stable) {
    b.stable = reading;
    if (b.stable == LOW) {
      tone(BUZZER, 2000, 15);        // click feedback
      return true;
    }
  }
  return false;
}

// ---- Time helpers ----
unsigned long clockSeconds() {
  return ((millis() - clockBaseMs) / 1000 + clockOffsetS) % 86400UL;
}

unsigned long stopwatchMs() {
  return swAccumMs + (running ? millis() - swStartMs : 0);
}

void print2(unsigned long v) {
  if (v < 10) lcd.print('0');
  lcd.print(v);
}

// Prints mm:ss.cc (or h:mm:ss.cc past one hour)
void printDuration(unsigned long ms) {
  unsigned long cs = (ms / 10) % 100;
  unsigned long s  = (ms / 1000) % 60;
  unsigned long m  = (ms / 60000UL) % 60;
  unsigned long h  = ms / 3600000UL;
  if (h > 0) { lcd.print(h); lcd.print(':'); }
  print2(m); lcd.print(':');
  print2(s); lcd.print('.');
  print2(cs);
}

void line(uint8_t row, const char *text) {
  lcd.setCursor(0, row);
  lcd.print(text);
}

// ---- Screens ----
void drawClock() {
  unsigned long t = clockSeconds();
  line(0, "[CLOCK]  A:H B:M");
  lcd.setCursor(0, 1);
  lcd.print("    ");
  print2(t / 3600); lcd.print(':');
  print2((t / 60) % 60); lcd.print(':');
  print2(t % 60);
  lcd.print("    ");
}

void drawStopwatch() {
  line(0, running ? "[STOPWATCH]  RUN" : "[STOPWATCH] STOP");
  lcd.setCursor(0, 1);
  lcd.print("  ");
  printDuration(stopwatchMs());
  lcd.print("      ");
}

void drawLaps() {
  lcd.setCursor(0, 0);
  lcd.print("[LAPS] ");
  lcd.print(lapCount);
  lcd.print(" saved    ");
  lcd.setCursor(0, 1);
  if (lapCount == 0) {
    lcd.print("No laps yet     ");
  } else {
    lcd.print("Lap ");
    lcd.print(lapView + 1);
    lcd.print(": ");
    printDuration(laps[lapView]);
    lcd.print("   ");
  }
}

// ---- Button actions per screen ----
void handleButtons() {
  if (pressed(buttons[0])) {
    screen = (Screen)((screen + 1) % 3);
    lapView = 0;
    lcd.clear();
  }

  bool a = pressed(buttons[1]);
  bool b = pressed(buttons[2]);

  switch (screen) {
    case CLOCK:
      if (a) clockOffsetS = (clockOffsetS + 3600) % 86400UL;
      if (b) {
        // +1 minute, wrapping inside the current hour
        unsigned long t = clockSeconds();
        unsigned long minute = (t / 60) % 60;
        clockOffsetS += (minute == 59) ? (86400UL - 59UL * 60) : 60;
        clockOffsetS %= 86400UL;
      }
      break;

    case STOPWATCH:
      if (a) {
        if (running) { swAccumMs += millis() - swStartMs; running = false; }
        else         { swStartMs = millis(); running = true; }
      }
      if (b) {
        if (running) {
          if (lapCount < MAX_LAPS) laps[lapCount++] = stopwatchMs();
        } else {
          swAccumMs = 0;
          lapCount = 0;
        }
      }
      break;

    case LAPS:
      if (a && lapCount > 0) lapView = (lapView + 1) % lapCount;
      break;
  }
}

void setup() {
  for (uint8_t i = 0; i < 3; i++) pinMode(buttons[i].pin, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  lcd.begin(16, 2);
  clockBaseMs = millis();
}

void loop() {
  handleButtons();

  static unsigned long lastDraw = 0;
  if (millis() - lastDraw >= 50) {       // ~20 screen refreshes per second
    lastDraw = millis();
    switch (screen) {
      case CLOCK:     drawClock();     break;
      case STOPWATCH: drawStopwatch(); break;
      case LAPS:      drawLaps();      break;
    }
  }
}
