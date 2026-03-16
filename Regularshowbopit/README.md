# Regular Show Bop It

This project is an Arduino Uno recreation of a Bop It-style game inspired by *Regular Show*. The game presents a command, plays a matching voice line through a DFPlayer Mini, waits for player input, and tracks score on a small LCD.

## Current Gameplay Loop

The game currently uses these states:

- `prestart`: waits in low-power mode until the start button wakes the device
- `mash`: player must press the buttons enough times before the timer expires
- `shake`: player must exceed the accelerometer threshold
- `hide`: placeholder state for the light sensor interaction
- `joke`: plays a joke clip and then moves on
- `win`: loops the win screen and win audio
- `lose`: loops the lose screen and lose audio

Difficulty ramps by reducing the delay window after successful rounds.

## Hardware In Use

- Arduino Uno
- DFPlayer Mini MP3 module
- Speaker connected to the DFPlayer Mini
- LIS3DH accelerometer
- NHD-C12832A1Z-FSW-FBW-3v3 display driven with `U8g2`
- Left button
- Right button
- Start button
- Photoresistor or other light sensor for the `hide` state

## Software Stack

This project is built with PlatformIO using the Arduino framework.

Key libraries in `platformio.ini`:

- `DFRobotDFPlayerMini`
- `Adafruit LIS3DH`
- `Adafruit Unified Sensor`
- `Adafruit BusIO`
- `U8g2`
- `PinChangeInterrupt`

## Audio Layout

The DFPlayer Mini expects audio files on the SD card in this structure:

```text
/MP3
  /1  -> action prompts
    001.mp3
    002.mp3
    ...
  /2  -> joke clips
    001.mp3
    002.mp3
    ...
```

Action prompts and jokes are selected in [`src/speakerConfig.hpp`](/abs/path/src/speakerConfig.hpp).

## Project Layout

- [`src/main.cpp`](/abs/path/src/main.cpp): game loop, state machine, display updates, and sensor-driven round logic
- [`src/speakerConfig.hpp`](/abs/path/src/speakerConfig.hpp): DFPlayer setup and sound selection
- [`src/init.c`](/abs/path/src/init.c) and [`src/init.h`](/abs/path/src/init.h): lower-level initialization support
- [`platformio.ini`](/abs/path/platformio.ini): PlatformIO environment and library dependencies

## Current Status

Implemented:

- Core state machine structure
- Display initialization and score updates
- DFPlayer Mini initialization and sound routing
- Accelerometer setup and shake detection
- Sleep/wake start flow

Still in progress:

- `hide` state sensor logic
- Finalized input debounce strategy
- Restart behavior in the middle of a round
- Full hardware validation and tuning
- Automated tests

## Building

Open the project in PlatformIO and build for the `uno` environment.

Typical commands:

```bash
pio run
pio run -t upload
```

## Notes

This repo is an active embedded project rather than a finished product. Some states and hardware interactions are still placeholders while the core game loop and device integration are being completed.
