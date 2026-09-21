# [SimpleCrush](https://sheasyve.dev/simplecrush)
A custom bitcrusher and downsampling VST3 plugin designed for audio destruction, vintage effects, and generating unique harmonics. 

Perfect for lo-fi textures and adding grit to bass, drums, and chords. Built from the ground up in C++ using the JUCE framework. 

<img width="342" height="372" alt="SimpleCrush Plugin UI" src="https://github.com/user-attachments/assets/69de983e-9d38-4fad-87d5-fac155d62054"/>

## Features

### Audio & DSP
* **Bit Depth Reduction:** Introduce quantization noise for a characteristic lo-fi sound. Highly effective for adding aggressive textures to bass and drums.
* **Sample Rate Reduction:** Lower the sample rate to induce classic aliasing artifacts and digital degradation.
* **Filter Controls:** Built-in high-pass and low-pass filters to tame undesirable frequencies generated during the crushing process.
* **Mix Control:** Seamlessly blend the processed signal with your dry audio for parallel processing.

### Workflow & UI
* **Preset Management:** A fully featured preset system including save, delete, load, folder selection, and a randomization tool for quick inspiration.
* **Custom Themes:** Choose from eight unique, persistent color palettes to match your DAW or personal aesthetic.
* **Dedicated Settings:** A centralized menu to manage theme selection, UI scale, and global plugin preferences.
* **Interactive Tooltips:** Hover over any control to reveal a concise description of its function, drastically reducing the learning curve.
* **Resizable UI:** A fully scalable, SVG-backed interface ensures crisp usability across any monitor size or resolution.

## Compatibility
* **Format:** 64-bit VST3 & Standalone Application
* **OS:** Windows and macOS (v1.3.1+)
* **DAWs:** Compatible with any major digital audio workstation that supports VST3 (Ableton Live, FL Studio, Reaper, Bitwig, Cubase, Logic Pro, etc.)

## Installation

### Windows
**Using the Installer (Recommended):**
1. Download the latest `SimpleCrush_v1.3.1_Windows.exe` from the Releases page.
2. Run the installer and follow the prompts.

**Manual ZIP Installation:**
1. Extract the downloaded archive (`SimpleCrush_v1.3.1.zip`).
2. Move the `SimpleCrush.vst3` folder into your DAW's designated VST3 plugin directory (typically `C:\Program Files\Common Files\VST3`).
3. Open the plugin in your DAW and redirect the preset folder within the plugin settings to wherever you placed your data files. 
4. Rescan for new plugins within your DAW.

### macOS
1. Download the unsigned macOS zip package (`SimpleCrush-macOS-Unsigned.zip`).
2. Extract the archive and place the plugin files into your system or user VST3 directory (`~/Library/Audio/Plug-Ins/VST3/` or `/Library/Audio/Plug-Ins/VST3/`).
3. Because the build is unsigned, you will need to bypass Gatekeeper by authenticating it via Terminal. Run the following command:
   ```bash
   sudo codesign --force --deep --sign - /Library/Audio/Plug-Ins/VST3/SimpleCrush.vst3
