# Multi-Screen Clock & Stopwatch

**EENG312 Term Project, Eastern Mediterranean University (Fall 2023, Nov 2023 – Jan 2024).** Team project with a classmate.

A dual-function timekeeping system that works as both a **real-time clock** and a **stopwatch**. One display switches between three screens: the clock, the stopwatch, and a list of recorded laps. The clock keeps counting in the background while the stopwatch is in use.

> The original course files are no longer available. This sketch is a rewrite of the same concept: the same three-screen design, written from scratch for this portfolio.

## Screens

```
┌────────────────┐   ┌────────────────┐   ┌────────────────┐
│[CLOCK]  A:H B:M│   │[STOPWATCH]  RUN│   │[LAPS] 3 saved  │
│    14:05:37    │──►│  01:23.45      │──►│Lap 1: 00:41.08 │──► back to clock
└────────────────┘   └────────────────┘   └────────────────┘
        MODE button cycles through the screens
```

## Controls

| Button | Clock screen | Stopwatch screen | Laps screen |
|--------|--------------|------------------|-------------|
| **MODE** | Next screen | Next screen | Next screen |
| **A** | Hour +1 | Start / stop | Next lap |
| **B** | Minute +1 | Lap (running) / reset (stopped) | n/a |

Every press gives a short click from the buzzer.

## Components

| Qty | Part |
|----:|------|
| 1 | Arduino UNO |
| 1 | 16×2 character LCD (HD44780) + 10 kΩ contrast potentiometer |
| 3 | Push buttons |
| 1 | Buzzer |

## Wiring

| Part | Pin | Arduino |
|------|-----|---------|
| LCD | RS / E | D12 / D11 |
| LCD | D4 / D5 / D6 / D7 | D5 / D4 / D3 / D2 |
| LCD | RW | GND |
| LCD | V0 | Potentiometer middle pin |
| Button MODE | to GND | D8 |
| Button A | to GND | D9 |
| Button B | to GND | D10 |
| Buzzer | + | D7 |

Buttons use the internal pull-up resistors, so no external resistors are needed.

## How it works

- **One time base:** both the clock and the stopwatch are computed from `millis()`. The clock stores an offset (the time you set). The stopwatch stores the time accumulated from earlier runs plus the start time of the current run, so pausing and resuming never loses time.
- **Non-blocking design:** the loop reads the buttons on every pass and redraws the screen about 20 times a second. There are no `delay()` calls, so the buttons always respond instantly.
- **Debounced buttons:** each button fires once per press, after its signal has been stable for 25 ms.
- **Laps:** up to 5 lap times are stored in an array and can be scrolled on the Laps screen. Resetting the stopwatch clears them.

## What I learned

- Building more than one function on a single microcontroller and display
- Timekeeping with `millis()` without drift from `delay()`
- Designing a small menu / multi-screen user interface
- Working on an engineering project as a team
