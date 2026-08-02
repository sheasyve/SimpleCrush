#pragma once
#include <JuceHeader.h>

class PluginControls : public juce::Component {
public:
    PluginControls(juce::AudioProcessorValueTreeState &apvts);
    ~PluginControls() override = default;

    void resized() override;
    juce::Rectangle<int> logoBounds;
    juce::Slider hpSlider, lpSlider, bitSlider, rateSlider, mixSlider;
    juce::Label hpLabel, lpLabel, bitLabel, rateLabel, mixLabel;

private:
    void lookAndFeelChanged() override;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hpAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lpAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginControls)
};