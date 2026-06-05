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
    mixSlider.setSliderStyle(juce::Slider::LinearVertical);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    mixSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    mixSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF593D));
    mixSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    mixSlider.setColour(juce::Slider::trackColourId, juce::Colours::red);
    // Format display to show percentage (0-100%)
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

    setSize(300, 200);
}

MyReduxEditor::~MyReduxEditor()
{
}

void MyReduxEditor::paint(juce::Graphics &g)
{
    juce::Colour centerColor = juce::Colour(0xFF2A2A2A);
    juce::Colour edgeColor = juce::Colour(0xFF111111);

    auto center = getLocalBounds().getCentre().toFloat();
    float radius = juce::jmax(getWidth(), getHeight()) * 0.7f;

    juce::ColourGradient gradient(centerColor, center.x, center.y, 
                                  edgeColor, center.x, center.y + radius, 
                                  true); // true makes it radial

    g.setGradientFill(gradient);
    g.fillAll();
}

void MyReduxEditor::resized()
{
    auto bounds = getLocalBounds().reduced(20);

    auto mixArea = bounds.removeFromRight(45); 
    mixLabel.setBounds(mixArea.removeFromTop(25)); 
    mixSlider.setBounds(mixArea); 

    const int gap = 15; 
    bounds.removeFromRight(gap); 

    auto columnWidth = (bounds.getWidth() - gap) / 2;
    
    auto leftColumn = bounds.removeFromLeft(columnWidth);
    bounds.removeFromLeft(gap); 
    auto rightColumn = bounds.removeFromLeft(columnWidth); 

    bitLabel.setBounds(leftColumn.removeFromTop(25));
    bitSlider.setBounds(leftColumn);

    rateLabel.setBounds(rightColumn.removeFromTop(25));
    rateSlider.setBounds(rightColumn);
}