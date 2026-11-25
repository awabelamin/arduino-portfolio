/*
  Blink: the "Hello, World!" of microcontrollers.
  Toggles the on-board LED (pin 13) once per second.
*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
