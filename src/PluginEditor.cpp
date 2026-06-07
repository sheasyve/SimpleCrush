#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace PluginTheme
{
    const juce::Colour sliderFill    = juce::Colour(0xFFFDB813); // Warm Amber
    const juce::Colour sliderOutline = juce::Colour(0xFF8B5A2B); // Brass
    const juce::Colour sliderThumb   = juce::Colour(0xFFFDB813); // Warm Amber
    const juce::Colour sliderTrack   = juce::Colour(0xFF3E2723); // Dark Brown
    const juce::Colour labelText     = juce::Colour(0xFFEFEBE9); // Cream
    const juce::Colour bgCenter      = juce::Colour(0xFF2D2D2D); // Soft Charcoal
    const juce::Colour bgEdge        = juce::Colour(0xFF1A1A1A); // Dark Charcoal

    /*
    const juce::Colour sliderFill    = juce::Colours::red;
    const juce::Colour sliderOutline = juce::Colour(0xFF593D);
    const juce::Colour sliderThumb   = juce::Colours::red;
    const juce::Colour sliderTrack   = juce::Colours::red;
    const juce::Colour labelText     = juce::Colours::white;
    const juce::Colour bgCenter      = juce::Colour(0xFF2A2A2A);
    const juce::Colour bgEdge        = juce::Colour(0xFF111111);
    */
}

// ==============================================================================

// The GUI of the plugin.

MyReduxEditor::MyReduxEditor(MyReduxProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // --- Bit Depth Knob ---
    bitSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bitSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    bitSlider.setColour(juce::Slider::rotarySliderFillColourId, PluginTheme::sliderFill);
    bitSlider.setColour(juce::Slider::rotarySliderOutlineColourId, PluginTheme::sliderOutline);
    bitSlider.setColour(juce::Slider::thumbColourId, PluginTheme::sliderThumb);
    bitSlider.setColour(juce::Slider::trackColourId, PluginTheme::sliderTrack);

    addAndMakeVisible(bitSlider);
    bitAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "BITS", bitSlider);

    // --- Bit Depth Label ---
    bitLabel.setText("BIT DEPTH", juce::dontSendNotification);
    bitLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    bitLabel.setJustificationType(juce::Justification::centred);
    bitLabel.setColour(juce::Label::textColourId, PluginTheme::labelText);

    addAndMakeVisible(bitLabel);

    // --- Downsample Knob ---
    rateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    rateSlider.setColour(juce::Slider::rotarySliderFillColourId, PluginTheme::sliderFill);
    rateSlider.setColour(juce::Slider::rotarySliderOutlineColourId, PluginTheme::sliderOutline);
    rateSlider.setColour(juce::Slider::thumbColourId, PluginTheme::sliderThumb);
    rateSlider.setColour(juce::Slider::trackColourId, PluginTheme::sliderTrack);

    addAndMakeVisible(rateSlider);
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "RATE", rateSlider);

    // --- Downsample Label ---
    rateLabel.setText("SAMPLE RATE", juce::dontSendNotification);
    rateLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    rateLabel.setJustificationType(juce::Justification::centred);
    rateLabel.setColour(juce::Label::textColourId, PluginTheme::labelText);

    addAndMakeVisible(rateLabel);

    // --- Mix Knob ---
    mixSlider.setSliderStyle(juce::Slider::LinearVertical);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    mixSlider.setColour(juce::Slider::rotarySliderFillColourId, PluginTheme::sliderFill);
    mixSlider.setColour(juce::Slider::rotarySliderOutlineColourId, PluginTheme::sliderOutline);
    mixSlider.setColour(juce::Slider::thumbColourId, PluginTheme::sliderThumb);
    mixSlider.setColour(juce::Slider::trackColourId, PluginTheme::sliderTrack);
    
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
    mixLabel.setColour(juce::Label::textColourId, PluginTheme::labelText);

    addAndMakeVisible(mixLabel);

    setSize(300, 200);
}

MyReduxEditor::~MyReduxEditor()
{
}

void MyReduxEditor::paint(juce::Graphics &g)
{
    auto center = getLocalBounds().getCentre().toFloat();
    float radius = juce::jmax(getWidth(), getHeight()) * 0.7f;

    juce::ColourGradient gradient(PluginTheme::bgCenter, center.x, center.y, 
                                  PluginTheme::bgEdge, center.x, center.y + radius, 
                                  true); // true makes it radial

    g.setGradientFill(gradient);
    g.fillAll();
}

void MyReduxEditor::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    auto mixArea = bounds.removeFromRight(45); 
    const int gap = 15; 
    auto columnWidth = (bounds.getWidth() - gap) / 2;
    mixLabel.setBounds(mixArea.removeFromTop(25)); 
    mixSlider.setBounds(mixArea); 
    bounds.removeFromRight(gap); 
    auto leftColumn = bounds.removeFromLeft(columnWidth);
    bounds.removeFromLeft(gap); 
    auto rightColumn = bounds.removeFromLeft(columnWidth); 
    bitLabel.setBounds(leftColumn.removeFromTop(25));
    bitSlider.setBounds(leftColumn);
    rateLabel.setBounds(rightColumn.removeFromTop(25));
    rateSlider.setBounds(rightColumn);
}