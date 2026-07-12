# SimpleCrush

A custom bitcrusher and downsampling VST3 plugin designed for aggressive audio destruction, and downsampling. Perfect for lo-fi, and adding unique harmonics to bass, drums and chords. Built from the ground up in C++ using the JUCE framework. This plugin is in early development stages, but is fully functional with many more changes to come.

<img width="342" height="372" alt="Screenshot 2026-07-12 174344" src="https://github.com/user-attachments/assets/b777d1ed-ccf8-4b63-8ac1-c17cb36ebab2" />

## Features
* **Bit Depth Reduction:** Crush your signal dynamically from 16-bit down to 1-bit using amplitude quantization.
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
- More features, like a jitter knob and other randomization.
- Presets
  
## Distribution Plan
### VST File
- Adept users can place the distributed vst folder in their vst directory.
- This can be released immediately.
### Inno Setup (The Industry Standard)
- This is the most common tool for VST developers. It is a free, script-based installer creator that creates professional-looking, single-file .exe installers.
- I still need to implement this
## Planned Distribution Networks
- GitHub Release
- https://sheasyve.dev
- Free plugin hosting websites.
  
