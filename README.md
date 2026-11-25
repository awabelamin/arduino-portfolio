# Arduino Projects Portfolio

**Awab Elamin** · [LinkedIn](https://www.linkedin.com/in/awab-osama-mohamed-elamin/)

A collection of the embedded-systems projects I've built with Arduino since 2020: from first LED blinks to autonomous robot cars, assistive devices, and a color-based fruit sorter.

Every project folder is a ready-to-open Arduino sketch with its own README: what it does, the parts list, the wiring table, and how the code works.

## About this repository

These projects span about six years of work, from 2020 to 2026, built for university courses, term projects, and my own learning.

Most of the original source files and photos from those years were not kept. **The ideas, designs, and hardware are mine, and I built and ran every one of these projects. The code in this repository is a fresh rewrite of each project**, recreated from its original concept and documented properly so that each one can be rebuilt and understood today.

That means:
- The pin choices and wiring tables describe a clean, reproducible version of each build. They may differ in small details from the original circuits.
- The sketches follow good practice (non-blocking timing, debouncing, state machines) and are written to be read and reused.

---

## Projects

| # | Project | What it does | Key parts | Skills |
|---|---------|--------------|-----------|--------|
| 1 | [Line Follower Robot](projects/line_follower/) | Follows a black line on the floor autonomously | IR sensors, L298N, 4WD chassis | Sensor feedback, motor control |
| 2 | [Obstacle-Avoiding Car](projects/obstacle_avoiding_car/) | Drives on its own, scans left/right with a servo "head" and picks the clearest path | HC-SR04, SG90 servo, L298N | Decision logic, PWM, servo control |
| 3 | [Smart Blind Stick](projects/blind_stick/) | Walking stick that warns visually impaired users of nearby obstacles with sound and vibration | HC-SR04, buzzer, vibration motor | Assistive tech, distance mapping |
| 4 | [Distance Alarm](projects/distance_alarm/) | Parking-sensor style alarm: LEDs go green → yellow → red and the beeping speeds up as objects get closer | HC-SR04, 3 LEDs, buzzer | Non-blocking timing with `millis()` |
| 5 | [Multi-Screen Clock & Stopwatch](projects/multiscreen_stopwatch/) | EENG312 term project (Fall 2023): clock + stopwatch + lap list on one LCD with three screens | LCD 16×2, 3 buttons, buzzer | Multi-screen UI, `millis()` timekeeping |
| 6 | [Automatic Plant Watering](projects/plant_watering/) | Measures soil moisture and runs a pump only when the plant is dry | Soil sensor, relay, water pump | State machines, hysteresis |
| 7 | [Mini Weather & Soil Station](projects/mini_weather_station/) | Shows temperature, humidity, and soil moisture on a 16×2 LCD | DHT11, soil sensor, LCD 16×2 | Sensor fusion, LCD UI |
| 8 | [Lemon Color Sorter](projects/lemon_color_sorter/) | Detects green vs. yellow lemons with a color sensor and flips a servo gate to sort them | TCS3200, SG90 servo | Color sensing, calibration, classification |
| 9 | [Sequence Memory Game](projects/sequence_memory_game/) | Simon-style game: repeat a growing sequence of lights and tones. High score saved in EEPROM | 4 LEDs, 4 buttons, buzzer | Game logic, debouncing, EEPROM |
| 0 | [Basics](projects/basics/) | The foundations: blink, potentiometer dimmer, button toggle, traffic light | LEDs, buttons, potentiometer | Digital/analog I/O, PWM |

---

## Skills demonstrated

- **Microcontroller programming:** C/C++ on Arduino (AVR), digital & analog I/O, PWM, interrupts-free non-blocking timing with `millis()`
- **Sensors:** ultrasonic (HC-SR04), IR reflectance, color (TCS3200), temperature & humidity (DHT11), soil moisture
- **Actuators:** DC motors via H-bridge (L298N), servos, relays and pumps, buzzers, vibration motors
- **Displays:** 16×2 character LCD, LED indicators, multi-display output
- **System design:** state machines, sensor calibration, hysteresis, debouncing, persistent storage (EEPROM)
- **Applied areas:** robotics, assistive technology, smart agriculture, automation and sorting

## Tools

- Arduino UNO (ATmega328P)
- Arduino IDE 2.x
- Breadboards, jumper wires, multimeter

---

## Repository structure

```
arduino-portfolio/
├── projects/
│   ├── basics/                  # blink, pot dimmer, button toggle, traffic light
│   ├── line_follower/
│   ├── obstacle_avoiding_car/
│   ├── blind_stick/
│   ├── distance_alarm/
│   ├── multiscreen_stopwatch/
│   ├── plant_watering/
│   ├── mini_weather_station/
│   ├── lemon_color_sorter/
│   └── sequence_memory_game/
├── LICENSE
└── README.md
```

Each project folder contains:
- `<project_name>.ino`: the sketch (Arduino requires the file name to match the folder name)
- `README.md`: description, components, wiring, how it works

## How to run a project

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Open the project's `.ino` file.
3. Install any library listed in the project's README (**Sketch → Include Library → Manage Libraries**).
4. Wire the circuit using the README's wiring table.
5. Select **Board: Arduino UNO** and the right port, then click **Upload**.

---

## About me

I'm Awab Elamin, an engineer who enjoys turning ideas into working hardware.

- LinkedIn: [linkedin.com/in/awab-osama-mohamed-elamin](https://www.linkedin.com/in/awab-osama-mohamed-elamin/)

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).
