# Sequence Memory Game

A breadboard memory game inspired by the [Human Benchmark Sequence Memory test](https://humanbenchmark.com/tests/sequence). The Arduino flashes a sequence of lights and tones, and you repeat it with the buttons. Every round adds one more step and plays a little faster. The **high score is saved in EEPROM**, so it survives power-off.

<!-- ![Demo](../../docs/media/sequence_memory_game/demo.gif) -->

## Components

| Qty | Part |
|----:|------|
| 1 | Arduino UNO |
| 4 | LEDs (red, green, blue, yellow) + 4 × 220 Ω resistors |
| 4 | Push buttons |
| 1 | Passive buzzer |
| 1 | Breadboard + jumper wires |

## Wiring

| Pad | LED (via 220 Ω) | Button | Tone |
|-----|-----------------|--------|------|
| Red | D2 | D6 | C4 (262 Hz) |
| Green | D3 | D7 | E4 (330 Hz) |
| Blue | D4 | D8 | G4 (392 Hz) |
| Yellow | D5 | D9 | C5 (523 Hz) |
| Buzzer | D10 | n/a | n/a |

Each button connects its pin to **GND**. The code uses the internal pull-up resistors (`INPUT_PULLUP`), so no external resistors are needed.

## How to play

1. Power on. The LEDs chase slowly. Press any button to start.
2. Watch the sequence, then repeat it.
3. Each correct round adds one step and plays faster.
4. A wrong button, or 5 s without pressing, ends the game.
5. Your score blinks back: each long flash of all LEDs = 10, each short green flash = 1.

**Reset the high score:** hold the red button while powering on.

## How it works

- **Sequence:** an array that grows by one random pad per round (`random(4)`). It is seeded with `micros()` at the moment the player presses start, since human timing is unpredictable.
- **Input:** buttons are debounced (20 ms), and each pad lights up and plays its tone while held, which gives immediate feedback.
- **Difficulty:** step time drops from 500 ms to a 200 ms floor as the level rises.
- **Persistence:** `EEPROM.put/get` stores the high score in the ATmega328P's non-volatile memory.

## What I learned

- Game state and flow on a microcontroller
- Button debouncing and pull-up inputs
- Generating tones with `tone()`
- Saving data across resets with EEPROM
