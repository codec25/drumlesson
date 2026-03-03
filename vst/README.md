# Drum Academy VST3

JUCE-based drum instrument plugin with built-in synthesis and practice controls.

## Included Features
- Playable drum kit from MIDI input (GM drum note map)
- Per-drum volume controls (kick/snare/hats/toms/crash/ride)
- Metronome on/off + metronome level
- Built-in backing groove on/off + backing level
- Loop bar range (`Loop Start` / `Loop End`) + loop toggle
- `Free Play` / `Training` mode selector
- Hi-hat pedal hold behavior via MIDI CC4
- Persistent parameter state (saved in DAW project)
- UI credit line: `by Moise T. Sahouo`

## Build (macOS)
1. Install CMake and JUCE.
2. Configure:
   ```bash
   cmake -S . -B build -DJUCE_DIR=/absolute/path/to/JUCE
   ```
3. Build:
   ```bash
   cmake --build build --config Release
   ```
4. Outputs:
   - VST3: `build/DrumAcademy_artefacts/Release/VST3/Drum Academy.vst3`
   - Standalone app: `build/DrumAcademy_artefacts/Release/Standalone/Drum Academy.app`

## MIDI Map
- Kick: `36`
- Snare: `38`, `40`
- Closed hat: `42`
- Open hat: `46` (converted to closed when pedal is held)
- Hat pedal: `44`
- Toms: `45` (low), `47/48` (mid), `50` (high)
- Crash: `49`, `57`
- Ride: `51`, `59`
- Hi-hat pedal hold: `CC4 >= 64`

## Downloadable Bundles
- Web app offline bundle:
  - `/Users/mrjovi/Documents/Education/my_game/test/drum-academy-offline.zip`
- VST source bundle:
  - `/Users/mrjovi/Documents/Education/my_game/test/drum-academy-vst-source.zip`
