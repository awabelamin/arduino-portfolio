# Obstacle-Avoiding Car

An autonomous 4WD robot car. It drives forward until it detects an obstacle. Then it backs up, turns its ultrasonic "head" left and right with a servo, and drives toward the side with more free space.

<!-- ![Demo](../../docs/media/obstacle_avoiding_car/demo.gif) -->

## Components

| Qty | Part |
|----:|------|
| 1 | Arduino UNO |
| 1 | 4WD car chassis + 4 DC gear motors |
| 1 | L298N motor driver |
| 1 | HC-SR04 ultrasonic sensor |
| 1 | SG90 servo |
| 1 | 2 × 18650 battery pack (~7.4 V) |

## Wiring

| Part | Pin | Arduino |
|------|-----|---------|
| L298N | ENA (left speed) | D5 |
| L298N | IN1 / IN2 | D7 / D8 |
| L298N | ENB (right speed) | D6 |
| L298N | IN3 / IN4 | D4 / D2 |
| L298N | 12V / GND | Battery + / − (GND also to Arduino GND) |
| L298N | 5V out | Arduino 5V (only with the 5V-EN jumper on) |
| Servo | Signal | D3 |
| HC-SR04 | TRIG / ECHO | A0 / A1 |

The two left motors are wired in parallel to OUT1/OUT2, and the two right motors to OUT3/OUT4.

> **Why not pins 9/10?** The `Servo` library uses Timer1, which disables `analogWrite()` on pins 9 and 10, so the motor speed pins are on 5 and 6.

## How it works

```
        ┌──────────────┐
        │ Read distance│◄──────────────────┐
        └──────┬───────┘                   │
     > 25 cm?  │                           │
      yes ─────┴──── no                    │
       │              │                    │
  Drive forward   Stop → reverse           │
       │          Look left, look right    │
       │          Turn toward more space ──┘
       └───────────────────────────────────┘
```

- **Differential steering:** spinning the left and right wheels in opposite directions turns the car in place.
- A missing echo (nothing within range) is treated as "clear" rather than an error.

## What I learned

- Driving DC motors with an H-bridge (direction + PWM speed)
- Timer conflicts between libraries (`Servo` vs `analogWrite`)
- Writing simple autonomous decision logic
- Shared ground and separate power for motors vs. logic
