#pragma once
#include "ui/style/plugin_theme.h"
#include "ui/style/svg.h"
#include <JuceHeader.h>

class PluginSettings : public juce::Component {

public:
    PluginSettings();
    ~PluginSettings() override = default;

    void paint(juce::Graphics &g) override;
    void resized() override;
    void setInitialTheme(int themeId);
    void setInitialFontSize(int fontSizeId);
    void updateIconColors(juce::Colour normal, juce::Colour hover);
    void setMenuOpen(bool isOpen);
    bool isMenuOpen() const { return isSettingsVisible; }
    // Callbacks
    std::function<void(bool)> onSettingsToggled;
    std::function<void(int)> onThemeChanged;
    std::function<void(int)> onFontSizeChanged;
    // State helper
    void updateMenuVisibility();

    // Components
    juce::Colour textColor = juce::Colours::white;
    juce::Font textFont;
    juce::DrawableButton settingsButton{"Settings", juce::DrawableButton::ImageFitted};
    juce::Label themeLabel;
    juce::ComboBox themeSelector;
    juce::Label fontSizeLabel;
    juce::ComboBox fontSizeSelector;
    juce::Label infoLabel;
    juce::TextButton infoButton;
    std::unique_ptr<juce::DrawablePath> drawableGear;
    std::unique_ptr<juce::DrawablePath> drawableGearHover;

    void setThemeStyle(juce::Colour newColor, juce::Font newFont) {
        textColor = newColor;
        textFont = newFont;
        repaint();
    }

private:
    bool isSettingsVisible = false;
    bool isPresetsVisible = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginSettings)
};