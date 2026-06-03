#include "PluginProcessor.h"
#include "PluginEditor.h"

// The GUI of the plugin.

MyReduxEditor::MyReduxEditor (MyReduxProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p){
    
    // --- Bit Depth Knob ---
    bitSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bitSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    
    bitSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    bitSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff4a0000)); // Dark red outline
    bitSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    bitSlider.setColour(juce::Slider::trackColourId, juce::Colours::red); 
    
    addAndMakeVisible(bitSlider);
    bitAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "BITS", bitSlider);

    // --- Downsample Knob ---
    rateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    
    rateSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    rateSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff4a0000)); 
    rateSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    rateSlider.setColour(juce::Slider::trackColourId, juce::Colours::red); 
    
    addAndMakeVisible(rateSlider);
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "RATE", rateSlider);

    setSize (300, 200);
}

MyReduxEditor::~MyReduxEditor(){
}

void MyReduxEditor::paint (juce::Graphics& g){
    g.fillAll (juce::Colour (0xff2d2d2d)); // Simple dark grey background
}

void MyReduxEditor::resized(){
    // Position them side by side
    auto bounds = getLocalBounds().reduced(20);
    auto width = bounds.getWidth() / 2;
    
    bitSlider.setBounds(bounds.removeFromLeft(width));
    rateSlider.setBounds(bounds);
}