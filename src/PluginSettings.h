#pragma once
#include <JuceHeader.h>

class PluginSettings : public juce::Component {
public:
    PluginSettings();
    ~PluginSettings() override = default;

    void paint(juce::Graphics &g) override;
    void resized() override;
    void setInitialTheme(int themeId);
    void updateIconColors(juce::Colour normal, juce::Colour hover);

    // Callbacks
    std::function<void(bool)> onSettingsToggled;
    std::function<void(bool)> onPresetsToggled;
    std::function<void(int)> onThemeChanged;

    // Preset Page Components
    juce::Label presetLabel;
    juce::ComboBox presetSelector;

    // State helper
    void updateMenuVisibility();

    // Components
    juce::DrawableButton settingsButton{"Settings", juce::DrawableButton::ImageFitted};
    juce::DrawableButton presetsButton{"Presets", juce::DrawableButton::ImageFitted};
    juce::Label themeLabel;
    juce::ComboBox themeSelector;
    juce::Label settingLabel;
    juce::ComboBox settingSelector;
    juce::Label infoLabel;
    juce::TextButton linkButton;
    std::unique_ptr<juce::DrawablePath> drawableGear;
    std::unique_ptr<juce::DrawablePath> drawableGearHover;
    std::unique_ptr<juce::DrawablePath> drawableList;
    std::unique_ptr<juce::DrawablePath> drawableListHover;

private:
    bool isSettingsVisible = false;
    bool isPresetsVisible = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginSettings)
};