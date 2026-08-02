#pragma once
#include "ui/main/plugin_controls.h"
#include "dsp/plugin_processor.h"
#include "ui/components/plugin_settings.h"
#include "ui/style/plugin_theme.h"
#include "ui/components/preset_menu.h"
#include <JuceHeader.h>

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
    void paintOverChildren(juce::Graphics &) override;
    void resized() override;

private:
    PluginTheme::SettingsBackground settingsOverlay;
    PluginTheme::SettingsBackground presetOverlay;
    MyReduxProcessor &audioProcessor;
    ThemeLookAndFeel themeLnF;
    PresetMenu presetMenu;
    PluginControls pluginControls;
    PluginSettings pluginSettings;
    juce::Rectangle<int> logoBounds;
    bool isSettingsVisible = false;
    bool isPresetsVisible = false;
    int currentThemeId = 1;
    int currentFontSizeId = 1;
    float getDynamicFontHeight() const;
    void updateTheme(int themeId);
    void updateFontSize(int fontSizeId);
    void setLabelsVisible(bool shouldBeVisible);
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