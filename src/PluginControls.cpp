#include "PluginControls.h"

PluginControls::PluginControls(juce::AudioProcessorValueTreeState& apvts) {
    setInterceptsMouseClicks(false, true);
    // --- High Pass Knob ---
    hpSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    hpSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(hpSlider);
    hpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "HPF", hpSlider);
    hpLabel.setText("HIGH PASS", juce::dontSendNotification);
    hpLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(hpLabel);

    // --- Low Pass Knob ---
    lpSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    lpSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(lpSlider);
    lpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "LPF", lpSlider);
    lpLabel.setText("LOW PASS", juce::dontSendNotification);
    lpLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lpLabel);

    // --- Bit Depth Knob ---
    bitSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bitSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    bitSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(bitSlider);
    bitAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "BITS", bitSlider);
    bitLabel.setText("BIT DEPTH", juce::dontSendNotification);
    bitLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(bitLabel);

    // --- Sample Rate Knob ---
    rateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    rateSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(rateSlider);
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "RATE", rateSlider);
    rateLabel.setText("SAMPLE RATE", juce::dontSendNotification);
    rateLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rateLabel);

    // --- Mix Knob ---
    mixSlider.setSliderStyle(juce::Slider::LinearVertical);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(mixSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "MIX", mixSlider);
    mixLabel.setText("MIX", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mixLabel);
}

void PluginControls::resized() {
    auto fullBounds = getLocalBounds();
    auto bounds = fullBounds.reduced(fullBounds.getWidth() * 0.066f);
    const int gap = bounds.getWidth() * 0.05f;

    // Mix Section (Right Side)
    auto mixArea = bounds.removeFromRight(bounds.getWidth() * 0.17f);
    mixLabel.setBounds(mixArea.removeFromTop(30));
    mixSlider.setBounds(mixArea);

    bounds.removeFromRight(gap);
    logoBounds = bounds.removeFromTop(bounds.getHeight() * 0.12f);

    // --- Top Area: Filters (Small & Centered) ---
    auto topArea = bounds.removeFromTop(bounds.getHeight() * 0.40f);
    int filterKnobWidth = bounds.getWidth() * 0.35f;
    int topGap = gap / 3;
    int filterAreaWidth = (filterKnobWidth * 2) + topGap;
    auto filterArea = topArea.withSizeKeepingCentre(filterAreaWidth, topArea.getHeight());

    auto hpArea = filterArea.removeFromLeft(filterKnobWidth);
    filterArea.removeFromLeft(topGap);
    auto lpArea = filterArea;

    hpLabel.setBounds(hpArea.removeFromTop(30));
    hpSlider.setBounds(hpArea);
    lpLabel.setBounds(lpArea.removeFromTop(30));
    lpSlider.setBounds(lpArea);

    bounds.removeFromTop(bounds.getHeight() * 0.10f);

    // --- Bottom Area: Main Controls (Big) ---
    auto bottomArea = bounds;
    int bottomKnobWidth = bottomArea.getWidth() * 0.45f;
    int bottomGap = gap * 0.25f;
    int bottomTotalWidth = (bottomKnobWidth * 2) + bottomGap;
    auto centeredBottomArea = bottomArea.withSizeKeepingCentre(bottomTotalWidth, bottomArea.getHeight());

    auto bitArea = centeredBottomArea.removeFromLeft(bottomKnobWidth);
    centeredBottomArea.removeFromLeft(bottomGap);
    auto rateArea = centeredBottomArea;

    bitLabel.setBounds(bitArea.removeFromTop(30));
    bitSlider.setBounds(bitArea);
    rateLabel.setBounds(rateArea.removeFromTop(30));
    rateSlider.setBounds(rateArea);
}