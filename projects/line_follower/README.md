# Line Follower Robot

A 4WD robot that follows a black tape line on a light floor using two infrared reflectance sensors and closed-loop steering.

## Components

| Qty | Part |
|----:|------|
| 1 | Arduino UNO |
| 1 | 4WD car chassis + 4 DC gear motors |
| 1 | L298N motor driver |
| 2 | IR line sensor modules (TCRT5000 or similar) |
| 1 | 2 × 18650 battery pack (~7.4 V) |
| n/a | Black electrical tape for the track |

## Wiring

| Part | Pin | Arduino |
|------|-----|---------|
| L298N | ENA / IN1 / IN2 (left) | D5 / D7 / D8 |
| L298N | ENB / IN3 / IN4 (right) | D6 / D4 / D2 |
| Left IR sensor | OUT | A0 |
| Right IR sensor | OUT | A1 |
| IR sensors | VCC / GND | 5V / GND |

Mount the sensors at the front, about 1 cm above the floor, placed on **either side** of the line so both see white when the robot is centered.

## How it works

| Left sensor | Right sensor | Action |
|:-----------:|:------------:|--------|
| white | white | Go straight |
| **black** | white | Turn left |
| white | **black** | Turn right |
| **black** | **black** | Stop (junction or finish line) |

Black tape reflects little IR light and white floor reflects a lot. Each module's comparator turns that into a digital HIGH/LOW. During a turn, the inner wheel runs slightly backwards so the robot can handle sharp curves.

## Tuning tips

- Adjust each sensor's potentiometer until its LED changes exactly at the tape edge.
- If the robot overshoots curves, lower `BASE_SPEED`. If it is sluggish, raise `TURN_FAST`.
- Next step: 3–5 sensors + PID control for smoother, faster following.

## What I learned

- Reflective IR sensing and threshold tuning
- Closed-loop control: sensor input directly steering the motors
- Differential drive with an H-bridge
