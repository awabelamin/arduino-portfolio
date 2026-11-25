# Mini Weather & Soil Station

A small monitoring station that measures **temperature**, **humidity**, and **soil moisture** and shows them live on a 16×2 LCD. It also labels the soil as DRY / OK / WET so you know when a plant needs water.

<!-- ![LCD](../../docs/media/mini_weather_station/photo.jpg) -->

## Components

| Qty | Part |
|----:|------|
| 1 | Arduino UNO |
| 1 | DHT11 temperature & humidity sensor |
| 1 | Soil moisture sensor |
| 1 | 16×2 character LCD (HD44780) |
| 1 | 10 kΩ potentiometer (LCD contrast) |
| 1 | 220 Ω resistor (LCD backlight) |

**Library:** *DHT sensor library* by Adafruit (Arduino Library Manager).

## Wiring

| Part | Pin | Arduino |
|------|-----|---------|
| LCD | RS / E | D12 / D11 |
| LCD | D4 / D5 / D6 / D7 | D5 / D4 / D3 / D2 |
| LCD | RW | GND |
| LCD | V0 (contrast) | Potentiometer middle pin |
| LCD | A / K (backlight) | 5V via 220 Ω / GND |
| DHT11 | DATA | D7 |
| Soil sensor | AO | A0 |

> Using an I²C LCD backpack instead? Replace `LiquidCrystal` with `LiquidCrystal_I2C` and connect SDA→A4, SCL→A5.

## Display layout

```
┌────────────────┐
│T:24.5°C H:56%  │
│Soil:43%   OK   │
└────────────────┘
```

## How it works

- The DHT11 is read every 2 s (it can't be read faster than about once per second). Failed reads show `DHT read error` instead of garbage values.
- The soil sensor's raw analog value (0–1023) is converted to a 0–100 % scale with `map()` between calibrated dry and wet readings.
- A custom LCD character draws the `°` symbol.

## What I learned

- Reading digital (DHT11) and analog (soil) sensors together
- Driving an HD44780 LCD and creating custom characters
- Calibrating raw sensor readings into meaningful units
