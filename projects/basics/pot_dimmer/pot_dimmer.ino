/*
  Potentiometer LED Dimmer
  Turning the potentiometer changes the LED brightness.
    analogRead():  0..1023 (10-bit ADC)
    analogWrite(): 0..255  (8-bit PWM)
*/

const uint8_t POT_PIN = A0;
const uint8_t LED_PIN = 9;   // must be a PWM pin (~)

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int raw = analogRead(POT_PIN);
  int brightness = map(raw, 0, 1023, 0, 255);
  analogWrite(LED_PIN, brightness);

  Serial.print("Pot: ");
  Serial.print(raw);
  Serial.print("  PWM: ");
  Serial.println(brightness);
  delay(20);
}
