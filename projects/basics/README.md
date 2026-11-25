# Basics

The first sketches everyone builds, and the foundation for every project in this repo.

| Sketch | What it does | Concept |
|--------|--------------|---------|
| [`blink`](blink/blink.ino) | Blinks the on-board LED | Digital output, `delay()` |
| [`pot_dimmer`](pot_dimmer/pot_dimmer.ino) | A potentiometer controls LED brightness | Analog input (ADC), PWM output, `map()` |
| [`button_toggle`](button_toggle/button_toggle.ino) | Each button press toggles an LED | Digital input, internal pull-up, debouncing |
| [`traffic_light`](traffic_light/traffic_light.ino) | Red → red+yellow → green → yellow cycle | Sequencing outputs, helper functions |

## Wiring

| Sketch | Wiring |
|--------|--------|
| blink | Nothing needed (uses the on-board LED on D13) |
| pot_dimmer | Pot outer pins → 5V and GND, middle → A0. LED + 220 Ω → D9 |
| button_toggle | Button between D2 and GND. Uses on-board LED (D13) |
| traffic_light | Red, yellow, green LEDs (each with 220 Ω) → D4, D3, D2 |
