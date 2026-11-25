# Automatic Plant Watering System

Keeps a plant watered without human help. A soil-moisture sensor checks the soil, and when it gets too dry, a relay switches on a small water pump until the soil is moist again.

<!-- ![Setup](../../docs/media/plant_watering/photo.jpg) -->

## Components

| Qty | Part |
|----:|------|
| 1 | Arduino UNO |
| 1 | Soil moisture sensor (resistive or capacitive) |
| 1 | 5 V relay module |
| 1 | Mini submersible water pump (3–6 V) + tubing |
| 1 | Separate pump power supply (battery pack or 5 V adapter) |
| 1 | Water container |

## Wiring

| Part | Pin | Arduino |
|------|-----|---------|
| Soil sensor | VCC / GND | 5V / GND |
| Soil sensor | AO (analog out) | A0 |
| Relay module | VCC / GND | 5V / GND |
| Relay module | IN | D7 |
| Pump | through relay COM/NO, powered by the separate supply | n/a |

> The pump runs from its own supply. The relay only switches it on and off, so the pump's current never flows through the Arduino.

## How it works

The system is a **state machine** with three states:

```
MONITORING ──(soil dry)──► WATERING ──(soil wet OR 5 s max)──► SOAKING ──(30 s)──► MONITORING
```

- **Hysteresis:** watering starts above `600` and stops only below `450`. A single threshold would make the pump flicker on and off around one value.
- **Safety timeout:** the pump never runs longer than 5 s at a time, which protects against an empty tank or a disconnected sensor.
- **Soak time:** water needs time to spread through the soil before the next reading means anything.

## Calibration

1. Open the Serial Monitor (9600 baud).
2. Note the reading with the probe in **dry** soil and in **freshly watered** soil.
3. Set `DRY_THRESHOLD` a bit below the dry value and `WET_THRESHOLD` a bit above the wet value.

## What I learned

- Designing with a state machine instead of nested `if`/`delay`
- Hysteresis and fail-safes for real-world actuators
- Isolating high-current loads with a relay
