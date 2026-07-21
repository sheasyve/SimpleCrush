#pragma once
#include <JuceHeader.h>

class PresetMenu : public juce::Component {
public:
    PresetMenu();
    ~PresetMenu() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    
    void updateIconColors(juce::Colour normal, juce::Colour hover);
    
    // External helper to close the menu without triggering callbacks
    void setMenuOpen(bool isOpen);
    bool isMenuOpen() const { return isPresetsVisible; }

    juce::Label presetLabel;
    juce::ComboBox presetSelector;
    std::function<void(bool)> onPresetsToggled;

private:
    void updateMenuVisibility();

    juce::DrawableButton presetsButton { "Presets", juce::DrawableButton::ImageFitted };
    std::unique_ptr<juce::DrawablePath> drawableList;
    std::unique_ptr<juce::DrawablePath> drawableListHover;

    bool isPresetsVisible = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetMenu)
};