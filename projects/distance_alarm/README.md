# Distance Alarm

A parking-sensor style alarm. An ultrasonic sensor measures the distance to the nearest object. Three LEDs show the zone (safe / warning / danger), and a buzzer beeps faster the closer the object gets.

I also built and presented a version of this project in my **Microcontrollers** course.

<!-- ![Demo](../../docs/media/distance_alarm/demo.gif) -->

## Components

| Qty | Part |
|----:|------|
| 1 | Arduino UNO |
| 1 | HC-SR04 ultrasonic sensor |
| 3 | LEDs (green, yellow, red) + 3 × 220 Ω resistors |
| 1 | Active or passive buzzer |
| 1 | Breadboard + jumper wires |

## Wiring

| Part | Pin | Arduino |
|------|-----|---------|
| HC-SR04 | VCC / GND | 5V / GND |
| HC-SR04 | TRIG | D9 |
| HC-SR04 | ECHO | D10 |
| Green LED (via 220 Ω) | + | D4 |
| Yellow LED (via 220 Ω) | + | D5 |
| Red LED (via 220 Ω) | + | D6 |
| Buzzer | + | D8 |

## How it works

1. The Arduino sends a 10 µs pulse on **TRIG**. The sensor emits an ultrasonic burst and holds **ECHO** high until the echo returns.
2. `pulseIn()` measures that time. Distance (cm) = time (µs) / 58.
3. Distance is split into three zones: **> 50 cm** safe, **20–50 cm** warning, **< 20 cm** danger.
4. The pause between beeps is mapped from distance (60–600 ms) and timed with `millis()`, so the sensor keeps reading while the buzzer beeps.

## What I learned

- How ultrasonic time-of-flight measurement works
- Mapping a sensor value to an output rate with `map()` and `constrain()`
- Non-blocking timing with `millis()` instead of `delay()`
