#pragma once
// --- Tooltip Definitions ---

struct PluginTooltips {
    static constexpr const char* highPass   = "Removes low frequencies.";
    static constexpr const char* lowPass    = "Tames harsh high frequencies.";
    static constexpr const char* bitDepth   = "Reduces bit depth for digital distortion.";
    static constexpr const char* sampleRate = "Reduces sample rate for lo-fi aliasing.";
    static constexpr const char* mix        = "Blends between the clean and crushed signals.";
};

struct PresetTooltips {
    static constexpr const char* menuToggle   = "Toggle the preset browser.";
    static constexpr const char* folder       = "Change the preset folder location.";
    static constexpr const char* deletePreset = "Delete the selected preset.";
    static constexpr const char* randomize    = "Randomize all knob values.";
    static constexpr const char* saveInput    = "Enter a name for your new preset.";
    static constexpr const char* saveButton   = "Save current settings as a preset.";
};

struct SettingsTooltips {
    static constexpr const char* settingsBtn = "Toggle the settings menu.";
    static constexpr const char* theme       = "Change the color scheme.";
    static constexpr const char* fontSize    = "Adjust the size of the text and labels.";
    static constexpr const char* tooltips    = "Show or hide these hover hints.";
    static constexpr const char* info        = "Visit the developer's website.";
};