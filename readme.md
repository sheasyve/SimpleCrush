# SimpleCrush💥

A custom bitcrusher and downsampling VST3 plugin designed for aggressive audio destruction, and downsampling. Perfect for lo-fi, and adding unique harmonics to bass, drums and chords. Built from the ground up in C++ using the JUCE framework. This plugin is in early development stages, but is fully functional with many more changes to come.

<img width="305" height="234" alt="image" src="https://github.com/user-attachments/assets/3e9c1161-4330-4cd3-a011-c8ca28e43374" />

## Features
* **Bit Depth Reduction:** Crush your signal dynamically from a pristine 16-bit down to a gritty 1-bit using amplitude quantization.
* **Sample Rate Downsampling:** Time-quantize your audio, rolling the sampling rate from 44.1 kHz down to 1.0 kHz.
* **Parallel Processing:** A built-in Mix knob utilizes linear interpolation to blend the crushed wet signal with the clean dry signal, allowing you to retain a bit of your drums, bass chords or other while adding a bit of spice.

## Compatibility
* **Format:** VST3 & Standalone Application
* **OS:** Cross Platform (Compiled via MSVC 2026 x64)
* **DAWs:** Compatible with any major digital audio workstation that supports the 64 bit VST3 format (Ableton Live, FL Studio, Reaper, Bitwig, Cubase, etc.)

## Build Instructions
This project is structured for modern C++ development using CMake. To compile the plugin locally:

1. Clone the repository to your local machine.
2. Open the project directory in Visual Studio Code.
3. Ensure you have the CMake Tools extension installed and an MSVC 2026 compiler toolchain active.
4. Run the build configuration (or execute a local PowerShell build script like `.\make.ps1`).

The compiler will generate a `.vst3` bundle and a standalone `.exe` test application inside your `build/` directory. 

## Roadmap
* Introduce a "Jitter" parameter to inject randomized clock variance into the downsampling loop, creating analog-style instability.
* Implement custom SVG graphics and override the default JUCE `LookAndFeel` for a premium, 3D-modeled hardware aesthetic.
