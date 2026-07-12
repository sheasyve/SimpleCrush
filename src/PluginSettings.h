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
    std::function<void(int)> onThemeChanged;

    // Components
    juce::DrawableButton settingsButton{"Settings", juce::DrawableButton::ImageFitted};
    juce::Label themeLabel;
    juce::ComboBox themeSelector;
    juce::Label infoLabel;
    juce::TextButton linkButton;
    std::unique_ptr<juce::DrawablePath> drawableGear;
    std::unique_ptr<juce::DrawablePath> drawableGearHover;

private:
    bool isSettingsVisible = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginSettings)
};