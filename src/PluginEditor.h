#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class MyReduxEditor : public juce::AudioProcessorEditor
{
public:
    MyReduxEditor(MyReduxProcessor &);
    ~MyReduxEditor() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    MyReduxProcessor &audioProcessor; // Reference back to the audio processor

    juce::Slider bitSlider; // GUI Components
    juce::Slider rateSlider;
    juce::Label bitLabel;
    juce::Label rateLabel;
    juce::Slider mixSlider;
    juce::Label mixLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyReduxEditor)
};