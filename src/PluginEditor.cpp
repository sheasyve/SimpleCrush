#include "PluginProcessor.h"
#include "PluginEditor.h"

// The GUI of the plugin.

MyReduxEditor::MyReduxEditor (MyReduxProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    // --- Bit Depth Knob ---
    bitSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bitSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    
    bitSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    bitSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF593D)); 
    bitSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    bitSlider.setColour(juce::Slider::trackColourId, juce::Colours::red); 
    
    addAndMakeVisible(bitSlider);
    bitAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "BITS", bitSlider);

    // --- Bit Depth Label ---
    bitLabel.setText("BIT DEPTH", juce::dontSendNotification);
    bitLabel.setFont(juce::FontOptions (14.0f, juce::Font::bold));
    bitLabel.setJustificationType(juce::Justification::centred);
    bitLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(bitLabel);

    // --- Downsample Knob ---
    rateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    
    rateSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    rateSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF593D)); 
    rateSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    rateSlider.setColour(juce::Slider::trackColourId, juce::Colours::red); 
    
    addAndMakeVisible(rateSlider);
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "RATE", rateSlider);

    // --- Downsample Label ---
    rateLabel.setText("SAMPLE RATE", juce::dontSendNotification);
    rateLabel.setFont(juce::FontOptions (14.0f, juce::Font::bold));
    rateLabel.setJustificationType(juce::Justification::centred);
    rateLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(rateLabel);

    setSize (300, 200);
}

MyReduxEditor::~MyReduxEditor() {
}

void MyReduxEditor::paint (juce::Graphics& g) {
    g.fillAll (juce::Colour (0xff2d2d2d)); // Simple dark grey background
}

void MyReduxEditor::resized() {
    // Position them side by side
    auto bounds = getLocalBounds().reduced(20);
    auto width = bounds.getWidth() / 2;
    
    auto leftColumn = bounds.removeFromLeft(width);
    auto rightColumn = bounds; 
    
    bitLabel.setBounds(leftColumn.removeFromTop(25));
    bitSlider.setBounds(leftColumn);
    
    rateLabel.setBounds(rightColumn.removeFromTop(25));
    rateSlider.setBounds(rightColumn);
}