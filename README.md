# Particula Primordial — Impact Engine

A procedural percussion synthesizer prototype for Ableton Live 12 Intro, delivered as a Windows VST3.

## Current prototype (v0.1)

- MIDI-triggered kick voice
- Exponential pitch-drop envelope
- Exponential amplitude decay
- Additive oscillator with up to 12 partials
- Adjustable inharmonicity
- Transient noise, body noise and soft saturation
- Parameter state persistence
- JUCE/CMake project and Windows GitHub Actions build workflow

## Important scope note

This is an original implementation inspired by the feature description supplied for Impaktor. It is not a clone, does not contain Impaktor code, and does not yet implement all advertised functions.

The first prototype deliberately prioritizes a stable, buildable synthesis path. The next milestones are:
1. Sample-accurate event scheduling and deterministic phase reset/phase-tail control.
2. Independent bipolar modifier curves and full envelope set.
3. True quadrature stereo synthesis with centered fundamental and spread upper partials.
4. Transient-split feedback delay network reverb.
5. Second oscillator, FM routing, velocity/random modulation matrix.
6. Waveform visualizer, preset system, parameter smoothing and CPU profiling.

## Build on Windows

Requirements:
- Visual Studio 2022 with **Desktop development with C++**
- CMake 3.22+
- Git
- Internet access on first configure (CMake downloads JUCE 8.0.6)

From a Developer PowerShell:

```powershell
cmake -B build -S . -A x64
cmake --build build --config Release --target ParticulaImpactEngine_VST3
```

The VST3 bundle is generated under:

```text
build/ParticulaImpactEngine_artefacts/Release/VST3/
```

Copy the `.vst3` bundle to:

```text
C:\Program Files\Common Files\VST3\
```

Then open Ableton Live, run a plug-in rescan, and load **Particula Impact Engine** on a MIDI track.

## GitHub Actions

Push this repository to GitHub. The workflow in `.github/workflows/build-windows.yml` builds the Windows VST3 and publishes it as a downloadable workflow artifact.

## Live 12 Intro compatibility

VST3 instruments are supported in Live 12 Intro. Max for Live devices (`.amxd`) are not part of this target.

## License

Source code: MIT License. JUCE is a separate dependency under its own license; see https://juce.com/legal/ for current terms.
