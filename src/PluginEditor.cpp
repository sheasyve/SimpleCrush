#include "PluginProcessor.h"
#include "PluginEditor.h"

// The GUI of the plugin.

MyReduxEditor::MyReduxEditor(MyReduxProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
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
    bitLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
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
    rateLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    rateLabel.setJustificationType(juce::Justification::centred);
    rateLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible(rateLabel);

    // --- Mix Knob ---
    mixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);

    mixSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    mixSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF593D));
    mixSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    mixSlider.setColour(juce::Slider::trackColourId, juce::Colours::red);
    // Format display to show percentage (0-100%)
    mixSlider.setTextValueSuffix(" %");
    mixSlider.textFromValueFunction = [](double value)
    {
        return juce::String(juce::roundToInt(value * 100.0));
    };

    addAndMakeVisible(mixSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "MIX", mixSlider);

    // --- Mix Label ---
    mixLabel.setText("MIX", juce::dontSendNotification);
    mixLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible(mixLabel);

    setSize(400, 200);
}

MyReduxEditor::~MyReduxEditor()
{
}

void MyReduxEditor::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(0xff2d2d2d)); 
}

void MyReduxEditor::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    auto mixArea = bounds.removeFromBottom(25); 
    mixLabel.setBounds(mixArea.removeFromLeft(40)); 
    mixSlider.setBounds(mixArea); 
    bounds.removeFromBottom(15); 

    // 2. Handle the Main Knobs (Bit Depth & Sample Rate)
    // Now we divide whatever space is left into TWO columns instead of three
    auto columnWidth = bounds.getWidth() / 2;
    auto leftColumn = bounds.removeFromLeft(columnWidth);
    auto rightColumn = bounds; 

    // --- BIT DEPTH ---
    bitLabel.setBounds(leftColumn.removeFromTop(25));
    bitSlider.setBounds(leftColumn);

    // --- SAMPLE RATE ---
    rateLabel.setBounds(rightColumn.removeFromTop(25));
    rateSlider.setBounds(rightColumn);
}