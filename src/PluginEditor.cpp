#include "PluginEditor.h"

#include "PluginProcessor.h"

namespace PluginTheme {
    const juce::Colour sliderFill = juce::Colour(0xFFAD7640);    // Retro Caramel
    const juce::Colour sliderOutline = juce::Colour(0xFF6B4226); // Chestnut
    const juce::Colour sliderThumb = juce::Colour(0xFFC99966);   // Vintage Tan
    const juce::Colour sliderTrack = juce::Colour(0xFF5C3822);   // Lighter warm mocha
    const juce::Colour labelText = juce::Colour(0xFFE3D3B5);
    const juce::Colour bgCenter = juce::Colour(0xFF2E1A0F);      // Darker rich cocoa
    const juce::Colour bgEdge = juce::Colour(0xFF120804);        // Deep espresso/almost black
} // namespace PluginTheme

MyReduxEditor::MyReduxEditor(MyReduxProcessor &p) : AudioProcessorEditor(&p), audioProcessor(p) {
    // --- High Pass Knob ---
    hpSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    hpSlider.setColour(juce::Slider::rotarySliderFillColourId, PluginTheme::sliderFill);
    hpSlider.setColour(juce::Slider::rotarySliderOutlineColourId, PluginTheme::sliderTrack);
    hpSlider.setColour(juce::Slider::trackColourId, PluginTheme::sliderTrack);
    hpSlider.setColour(juce::Slider::thumbColourId, PluginTheme::sliderThumb);

    addAndMakeVisible(hpSlider);
    hpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "HPF", hpSlider);

    hpLabel.setText("HIGH PASS", juce::dontSendNotification);
    hpLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    hpLabel.setJustificationType(juce::Justification::centred);
    hpLabel.setColour(juce::Label::textColourId, PluginTheme::labelText);
    addAndMakeVisible(hpLabel);

    // --- Bit Depth Knob ---
    bitSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bitSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    bitSlider.setColour(juce::Slider::rotarySliderFillColourId, PluginTheme::sliderFill);
    bitSlider.setColour(juce::Slider::rotarySliderOutlineColourId, PluginTheme::sliderTrack);
    bitSlider.setColour(juce::Slider::trackColourId, PluginTheme::sliderTrack);
    bitSlider.setColour(juce::Slider::thumbColourId, PluginTheme::sliderThumb);

    addAndMakeVisible(bitSlider);
    bitAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "BITS", bitSlider);

    bitLabel.setText("BIT DEPTH", juce::dontSendNotification);
    bitLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    bitLabel.setJustificationType(juce::Justification::centred);
    bitLabel.setColour(juce::Label::textColourId, PluginTheme::labelText);
    addAndMakeVisible(bitLabel);

    // --- Downsample Knob ---
    rateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    rateSlider.setColour(juce::Slider::rotarySliderFillColourId, PluginTheme::sliderFill);
    rateSlider.setColour(juce::Slider::rotarySliderOutlineColourId, PluginTheme::sliderTrack);
    rateSlider.setColour(juce::Slider::trackColourId, PluginTheme::sliderTrack);
    rateSlider.setColour(juce::Slider::thumbColourId, PluginTheme::sliderThumb);

    addAndMakeVisible(rateSlider);
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "RATE", rateSlider);

    rateLabel.setText("SAMPLE RATE", juce::dontSendNotification);
    rateLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    rateLabel.setJustificationType(juce::Justification::centred);
    rateLabel.setColour(juce::Label::textColourId, PluginTheme::labelText);
    addAndMakeVisible(rateLabel);

    // --- Low Pass Knob ---
    lpSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lpSlider.setColour(juce::Slider::rotarySliderFillColourId, PluginTheme::sliderFill);
    lpSlider.setColour(juce::Slider::rotarySliderOutlineColourId, PluginTheme::sliderTrack);
    lpSlider.setColour(juce::Slider::trackColourId, PluginTheme::sliderTrack);
    lpSlider.setColour(juce::Slider::thumbColourId, PluginTheme::sliderThumb);

    addAndMakeVisible(lpSlider);
    lpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "LPF", lpSlider);

    lpLabel.setText("LOW PASS", juce::dontSendNotification);
    lpLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    lpLabel.setJustificationType(juce::Justification::centred);
    lpLabel.setColour(juce::Label::textColourId, PluginTheme::labelText);
    addAndMakeVisible(lpLabel);

    // --- Mix Knob ---
    mixSlider.setSliderStyle(juce::Slider::LinearVertical);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mixSlider.setColour(juce::Slider::trackColourId, PluginTheme::sliderFill);
    mixSlider.setColour(juce::Slider::backgroundColourId, PluginTheme::sliderTrack);
    mixSlider.setColour(juce::Slider::thumbColourId, PluginTheme::sliderThumb);

    mixSlider.textFromValueFunction = [](double value) {
        return juce::String(juce::roundToInt(value * 100.0));
    };
    addAndMakeVisible(mixSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "MIX", mixSlider);

    mixLabel.setText("MIX", juce::dontSendNotification);
    mixLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::textColourId, PluginTheme::labelText);
    addAndMakeVisible(mixLabel);

    // --- Text Box Borders ---
    hpSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    bitSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    rateSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    lpSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    setSize(325, 300);
}

MyReduxEditor::~MyReduxEditor() {}

void MyReduxEditor::paint(juce::Graphics &g) {
    auto center = getLocalBounds().getCentre().toFloat();
    float radius = juce::jmax(getWidth(), getHeight()) * 0.7f;

    juce::ColourGradient gradient(PluginTheme::bgCenter, center.x, center.y, PluginTheme::bgEdge,
                                  center.x, center.y + radius, true);

    g.setGradientFill(gradient);
    g.fillAll();
}

void MyReduxEditor::resized() 
{
    auto bounds = getLocalBounds().reduced(20);
    const int gap = 15;

    // --- 1. Mix Section ---
    auto mixArea = bounds.removeFromRight(45);
    mixLabel.setBounds(mixArea.removeFromTop(25));
    mixSlider.setBounds(mixArea);

    // Add a gap specifically between the Mix slider and the rest of the UI
    bounds.removeFromRight(gap);

    // --- 2. Split vertically into Top (Filters) and Bottom (Main FX) ---
    auto topArea = bounds.removeFromTop(bounds.getHeight() * 0.45f);
    bounds.removeFromTop(10); // Vertical gap between the two rows

    // --- 3. Top Area: Filters ---
    int filterKnobWidth = 70;
    int filterAreaWidth = (filterKnobWidth * 2) + gap; 
    auto filterArea = topArea.withSizeKeepingCentre(filterAreaWidth, topArea.getHeight());
    auto hpArea = filterArea.removeFromLeft(filterKnobWidth);
    filterArea.removeFromLeft(gap); 
    auto lpArea = filterArea.removeFromLeft(filterKnobWidth);
    
    hpLabel.setBounds(hpArea.removeFromTop(20));
    hpSlider.setBounds(hpArea);
    lpLabel.setBounds(lpArea.removeFromTop(20));
    lpSlider.setBounds(lpArea);

    // --- 4. Bottom Area: Main Controls ---
    auto bottomArea = bounds;
    auto columnWidth = (bottomArea.getWidth() - gap) / 2;
    auto bitArea = bottomArea.removeFromLeft(columnWidth);
    bottomArea.removeFromLeft(gap); 
    auto rateArea = bottomArea.removeFromLeft(columnWidth);

    bitLabel.setBounds(bitArea.removeFromTop(25));
    bitSlider.setBounds(bitArea);
    rateLabel.setBounds(rateArea.removeFromTop(25));
    rateSlider.setBounds(rateArea);
}