# Lemon Color Sorter (Green vs. Yellow)

An automatic fruit sorter. A lemon rolls under a **TCS3200 color sensor**, the Arduino decides whether it is **yellow (ripe)** or **green (unripe)**, and a **servo flap** sends it into the matching box. The same idea is used on real fruit-grading lines.

## Components

| Qty | Part |
|----:|------|
| 1 | Arduino UNO |
| 1 | TCS3200 color sensor (with light-shielding housing) |
| 1 | SG90 servo + flap (cardboard / acrylic) |
| 1 | Ramp and two collection boxes |

## Wiring

| Part | Pin | Arduino |
|------|-----|---------|
| TCS3200 | VCC / GND | 5V / GND |
| TCS3200 | S0 / S1 | D4 / D5 |
| TCS3200 | S2 / S3 | D6 / D7 |
| TCS3200 | OUT | D8 |
| TCS3200 | OE | GND (always enabled) |
| Servo | Signal | D9 |

## How it works

### 1. Measuring color
The TCS3200 has an array of photodiodes with red, green, blue, and clear filters. Pins **S2/S3** select a filter, and the chip outputs a square wave whose frequency is proportional to the light intensity. `pulseIn()` measures the wave's period: **shorter period = more light**.

| S2 | S3 | Filter |
|----|----|--------|
| LOW | LOW | Red |
| HIGH | HIGH | Green |
| LOW | HIGH | Blue |

### 2. Classifying
Yellow light is a mix of red and green, so a **yellow lemon reflects a lot of red**, while a green lemon reflects much less red than green. The sorter uses one robust feature:

```
ratio = redPeriod / greenPeriod
ratio ≤ 1.15  → YELLOW
ratio > 1.15  → GREEN
```

Using a **ratio** instead of raw values makes the decision less sensitive to lemon size, distance, and ambient light.

### 3. Detecting a lemon
The area under the sensor is a dark (black) surface, so with no lemon all three periods are long. When their sum drops below `PRESENCE_MAX`, a lemon is in position.

### 4. Sorting
The servo flap tilts left (yellow) or right (green), waits for the lemon to drop, and returns to center. Counts are printed to the Serial Monitor.

## Calibration

1. Set `CALIBRATION_MODE = true` and upload.
2. Note `sum` and `R/G` with **no lemon**, a **yellow lemon**, and a **green lemon**.
3. Set `PRESENCE_MAX` between the empty and lemon sums, and `YELLOW_MAX_RATIO` between the yellow and green ratios.
4. Set `CALIBRATION_MODE = false` and upload again.

## What I learned

- How light-to-frequency color sensors work
- Feature engineering: picking a ratio that separates two classes reliably
- Calibrating a sensor for real lighting conditions
- Combining sensing, decision, and actuation into one automated process
