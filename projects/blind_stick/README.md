# Smart Blind Stick

An assistive device for visually impaired people: an ordinary walking stick fitted with an ultrasonic sensor. It warns the user about obstacles ahead with **sound and vibration**, and the warning gets faster as the obstacle gets closer.

<!-- ![Stick](../../docs/media/blind_stick/photo.jpg) -->

## Components

| Qty | Part |
|----:|------|
| 1 | Arduino UNO (or Nano, which fits better on a stick) |
| 1 | HC-SR04 ultrasonic sensor |
| 1 | Buzzer |
| 1 | Coin vibration motor + NPN transistor (e.g. 2N2222) + 1 kΩ resistor + diode |
| 1 | 9 V battery or power bank |
| 1 | Walking stick + tape / 3D-printed mount |

## Wiring

| Part | Pin | Arduino |
|------|-----|---------|
| HC-SR04 | VCC / GND | 5V / GND |
| HC-SR04 | TRIG | D9 |
| HC-SR04 | ECHO | D10 |
| Buzzer | + | D8 |
| Transistor base (via 1 kΩ) | B | D6 |
| Vibration motor | between 5V and transistor collector (diode across motor) | n/a |

> The motor is driven through a transistor because an Arduino pin can't supply enough current for it directly.

## How it works

| Distance | Feedback |
|----------|----------|
| > 100 cm or nothing detected | Silent |
| 25–100 cm | Beep + vibration pulses, faster as distance shrinks |
| < 25 cm | Continuous tone + vibration |

The pulse period is mapped from distance with `map()` and toggled with `millis()`, so the stick reacts immediately when the distance changes.

## What I learned

- Designing feedback for a user who can't see the device
- Driving inductive loads (motors) safely with a transistor and flyback diode
- Making a portable, battery-powered build
