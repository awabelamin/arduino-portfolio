/*
  Mini Weather & Soil Station
  ----------------------------
  Reads temperature and humidity (DHT11) and soil moisture, and shows
  them on a 16x2 LCD:

    T:24.5C  H:56%
    Soil:43%  OK

  Required library: "DHT sensor library" by Adafruit
  (also installs "Adafruit Unified Sensor").
*/

#include <LiquidCrystal.h>
#include <DHT.h>

// LCD (4-bit mode): RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const uint8_t DHT_PIN  = 7;
const uint8_t SOIL_PIN = A0;
DHT dht(DHT_PIN, DHT11);

// Calibrate: raw reading in dry air/soil and in water/wet soil
const int SOIL_DRY_RAW = 1000;
const int SOIL_WET_RAW = 400;

const unsigned long UPDATE_MS = 2000;   // DHT11 can't be read faster than ~1 s
unsigned long lastUpdate = 0;

byte degreeChar[8] = {
  B00110, B01001, B01001, B00110, B00000, B00000, B00000, B00000
};

int soilPercent() {
  int raw = analogRead(SOIL_PIN);
  int pct = map(raw, SOIL_DRY_RAW, SOIL_WET_RAW, 0, 100);
  return constrain(pct, 0, 100);
}

const char* soilStatus(int pct) {
  if (pct < 30) return "DRY";
  if (pct > 80) return "WET";
  return "OK ";
}

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, degreeChar);
  dht.begin();
  Serial.begin(9600);

  lcd.print("Weather Station");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(1500);
  lcd.clear();
}

void loop() {
  if (millis() - lastUpdate < UPDATE_MS) return;
  lastUpdate = millis();

  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  int soil   = soilPercent();

  // Line 1: temperature and humidity
  lcd.setCursor(0, 0);
  if (isnan(temp) || isnan(hum)) {
    lcd.print("DHT read error  ");
  } else {
    lcd.print("T:");
    lcd.print(temp, 1);
    lcd.write(byte(0));          // degree symbol
    lcd.print("C H:");
    lcd.print((int)hum);
    lcd.print("%   ");
  }

  // Line 2: soil moisture
  lcd.setCursor(0, 1);
  lcd.print("Soil:");
  lcd.print(soil);
  lcd.print("%  ");
  lcd.setCursor(11, 1);
  lcd.print(soilStatus(soil));
  lcd.print("  ");

  Serial.print("T="); Serial.print(temp);
  Serial.print(" H="); Serial.print(hum);
  Serial.print(" Soil="); Serial.print(soil); Serial.println("%");
}
