#pragma once
#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "PluginTheme.h"
#include "PluginControls.h"
#include "PluginSettings.h"

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
    MyReduxProcessor &audioProcessor; 
    ThemeLookAndFeel themeLnF;
    PluginControls pluginControls;
    PluginSettings pluginSettings;
    juce::Rectangle<int> logoBounds;
    bool isSettingsVisible = false;
    bool isPresetsVisible = false;
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