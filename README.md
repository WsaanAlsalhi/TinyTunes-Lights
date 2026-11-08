# 🎵 TinyTunes Lights

A cute little music box built on the **STM32F103C8T6 (Blue Pill)**.
Press a button, hear a kids' melody on the buzzer, and watch six LEDs dance to the notes!

Every button has its own **song** and its own **light pattern**. The pattern follows the pitch of the melody and always starts from the LED of the button you pressed.

> The onboard 0.96" ST7735 TFT screen is **not used** in this project.

---

## ✨ Features

- 6 buttons, 6 LEDs, 1 passive buzzer
- 6 classic kids' songs
- LED patterns are generated from the melody itself (note pitch), not a fixed animation
- Every song has a different pattern style
- Non-blocking code (no `delay()`): pressing another button instantly switches to its song

## 🎶 Songs and Patterns

| Button | Song | LED pattern |
|:---:|---|---|
| 1 | Twinkle Twinkle Little Star | **Pitch Follow**: one LED moves up and down with the pitch |
| 2 | Mary Had a Little Lamb | **Mirror**: two LEDs move in opposite directions |
| 3 | Jingle Bells | **Bar Meter**: higher note = more LEDs lit |
| 4 | Happy Birthday | **Expand**: light spreads from the start LED to both sides |
| 5 | Ode to Joy | **Bounce**: light bounces back and forth, pitch changes the jump |
| 6 | Row, Row, Row Your Boat | **Trail**: a head LED with a 3-LED tail follows the melody |

## 🧰 Hardware

| Part | Qty | Notes |
|---|:---:|---|
| STM32F103C8T6 Blue Pill board | 1 | The TFT screen is not used |
| LEDs | 6 | Any color |
| Resistors 220-330 Ω | 6 | One per LED |
| Push buttons | 6 | Momentary |
| **Passive** buzzer | 1 | An *active* buzzer cannot play melodies |
| Breadboard + jumper wires | - | |

## 🔌 Wiring

| Component | Pin |
|---|---|
| LED 1 → LED 6 | `PA0`, `PA1`, `PA2`, `PA3`, `PB0`, `PB1` (each through a resistor to GND) |
| Button 1 → Button 6 | `PB12`, `PB13`, `PB14`, `PB15`, `PA6`, `PA7` (other side to GND) |
| Buzzer (+) | `PA8` (other side to GND) |

Buttons use the internal `INPUT_PULLUP`, so **no external resistors are needed** for them.

Avoided pins: `PA11/PA12` (USB), `PA13/PA14` (SWD programming), `PC13` (onboard LED).

```
 Button n ──────── GND
    │
  PBxx/PAxx

 PAxx/PBxx ──[220Ω]──►|── GND      (LED)

 PA8 ──── Buzzer(+)    Buzzer(-) ──── GND
```

## 💻 Software Setup

1. Install **Arduino IDE**.
2. Install **STM32duino** core: *Boards Manager* → search `STM32 MCU based boards` (by STMicroelectronics).
3. Select:
   - **Board:** Generic STM32F1 series
   - **Board part number:** BluePill F103C8
   - **Upload method:** ST-Link (or Serial / USB bootloader, depending on your setup)
4. Open `TinyTunesLights.ino` and upload.

No external libraries are required.

## ▶️ Usage

Press any button to play its song. Press another button at any time to interrupt and switch songs. When a song finishes, all LEDs turn off.

## 🔧 Customization

| I want to... | Do this |
|---|---|
| Change song speed | Edit the `tempo` value in `songs[]` (lower = faster) |
| Change a pattern style | Edit the last field of that song in `songs[]` (e.g. `MIRROR` → `TRAIL`) |
| Add a new pattern | Add a name to `enum PatternMode`, then a new `case` in `showPattern()` |
| Add a new song | Create a notes array and a durations array, then add a line to `songs[]` |
| Loop songs forever | In `loop()`, replace `stopSong();` with `noteIdx = 0; startNote();` |
| Ignore buttons while playing | Change the button condition to `if (!playing && ...)` |

Note durations: `1` = whole, `2` = half, `4` = quarter, `8` = eighth.

## 🛠️ Troubleshooting

- **No melody, just a constant beep:** your buzzer is *active*. Use a *passive* buzzer.
- **A button does nothing:** check that one side goes to the pin and the other to GND.
- **Upload fails:** make sure you did not wire anything to `PA13/PA14`, and that the BOOT0 jumper is set correctly for your upload method.
- **LED too dim or too bright:** adjust the resistor value (220-330 Ω is a good range).


Free to use, modify and share. Have fun! 🎉
