#pragma once
#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "PluginTheme.h"
class ThemeLookAndFeel : public juce::LookAndFeel_V4 {
public:
    juce::FontOptions currentFont;

    juce::Font getLabelFont(juce::Label &) override { return juce::Font(currentFont); }
};

class MyReduxEditor : public juce::AudioProcessorEditor {
public:
    MyReduxEditor(MyReduxProcessor &);
    ~MyReduxEditor() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    MyReduxProcessor &audioProcessor; // Reference back to the audio processor
    ThemeLookAndFeel themeLnF;
    juce::Slider bitSlider; // GUI Components
    juce::Slider rateSlider;
    juce::Label bitLabel;
    juce::Label rateLabel;
    juce::Slider mixSlider;
    juce::Label mixLabel;
    juce::Slider hpSlider;
    juce::Label hpLabel;
    juce::Label lpLabel;
    juce::Slider lpSlider;
    juce::DrawableButton settingsButton{"Settings", juce::DrawableButton::ImageFitted};
    juce::ComboBox themeSelector;
    juce::Label themeLabel;
    juce::Label infoLabel;
    juce::Rectangle<int> logoBounds;
    std::string infotext;
    bool isSettingsVisible = false;
    int currentThemeId = 1;
    void updateTheme(int themeId);
    std::unique_ptr<juce::DrawablePath> drawableGear;
    std::unique_ptr<juce::DrawablePath> drawableGearHover;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hpAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lpAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> themeAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyReduxEditor)
};