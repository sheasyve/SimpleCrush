#include "PluginEditor.h"
#include "PluginProcessor.h"

// --- Theme Data Structure ---
namespace PluginTheme {
    struct Colors {
        juce::Colour sliderFill;
        juce::Colour sliderOutline;
        juce::Colour sliderThumb;
        juce::Colour sliderTrack;
        juce::Colour labelText;
        juce::Colour bgCenter;
        juce::Colour bgEdge;
    };

    inline Colors getColors(int themeId) {
        if (themeId == 2) { // Vaporwave!
            return {
                juce::Colour(0xFFFF2A6D), // Neon Pink (Fill)
                juce::Colour(0xFF05D9E8), // Cyan (Outline)
                juce::Colour(0xFF01FFF4), // Bright Cyan (Thumb)
                juce::Colour(0xFF240046), // Deep Purple (Track)
                juce::Colour(0xFF05D9E8), // Cyan (Text)
                juce::Colour(0xFF10002B), // Very Dark Purple (BG Center)
                juce::Colour(0xFF000000)  // Black (BG Edge)
            };
        }
        
        // Default: Retro Caramel (themeId == 1)
        return {
            juce::Colour(0xFFAD7640),
            juce::Colour(0xFF6B4226),
            juce::Colour(0xFFC99966),
            juce::Colour(0xFF5C3822),
            juce::Colour(0xFFE3D3B5),
            juce::Colour(0xFF2E1A0F),
            juce::Colour(0xFF120804)
        };
    }
} // namespace PluginTheme

// --- Constructor ---
MyReduxEditor::MyReduxEditor(MyReduxProcessor &p) : AudioProcessorEditor(&p), audioProcessor(p) {
    // --- High Pass Knob ---
    hpSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    hpSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(hpSlider);
    hpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "HPF", hpSlider);
    hpLabel.setText("HIGH PASS", juce::dontSendNotification);
    hpLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    hpLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(hpLabel);

    // --- Bit Depth Knob ---
    bitSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bitSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    bitSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(bitSlider);
    bitAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "BITS", bitSlider);
    bitLabel.setText("BIT DEPTH", juce::dontSendNotification);
    bitLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    bitLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(bitLabel);

    // --- Downsample Knob ---
    rateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    rateSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(rateSlider);
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "RATE", rateSlider);
    rateLabel.setText("SAMPLE RATE", juce::dontSendNotification);
    rateLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    rateLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rateLabel);

    // --- Low Pass Knob ---
    lpSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    lpSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(lpSlider);
    lpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "LPF", lpSlider);
    lpLabel.setText("LOW PASS", juce::dontSendNotification);
    lpLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    lpLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lpLabel);

    // --- Mix Knob ---
    mixSlider.setSliderStyle(juce::Slider::LinearVertical);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(mixSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "MIX", mixSlider);
    mixLabel.setText("MIX", juce::dontSendNotification);
    mixLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    mixLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mixLabel);

    // --- Settings Gear Button ---
    settingsButton.setButtonText(juce::String::fromUTF8("\xe2\x9a\x99")); 
    settingsButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(settingsButton);

    settingsButton.onClick = [this]() {
        isSettingsVisible = !isSettingsVisible;
        themeSelector.setVisible(isSettingsVisible);
        themeLabel.setVisible(isSettingsVisible);
        
        hpSlider.setVisible(!isSettingsVisible);
        bitSlider.setVisible(!isSettingsVisible);
        rateSlider.setVisible(!isSettingsVisible);
        lpSlider.setVisible(!isSettingsVisible);
        mixSlider.setVisible(!isSettingsVisible);
        
        resized(); // Force coordinate update for the overlay
        repaint();
    };

    // --- Theme Selector Overlay ---
    themeLabel.setText("THEME", juce::dontSendNotification);
    themeLabel.setJustificationType(juce::Justification::centred);
    addChildComponent(themeLabel); // Invisible by default

    themeSelector.addItem("Retro Caramel", 1);
    themeSelector.addItem("Vaporwave", 2);
    themeSelector.setSelectedId(1, juce::dontSendNotification);
    addChildComponent(themeSelector); // Invisible by default

    themeSelector.onChange = [this]() {
        updateTheme(themeSelector.getSelectedId());
    };

    // Apply the initial theme colors
    updateTheme(currentThemeId);

    setSize(325, 300);
}

MyReduxEditor::~MyReduxEditor() {}

// --- Theme Update Logic ---
void MyReduxEditor::updateTheme(int themeId) {
    currentThemeId = themeId;
    auto colors = PluginTheme::getColors(themeId);

    // 1. Update standard rotary sliders (unrolled for compiler safety)
    hpSlider.setColour(juce::Slider::rotarySliderFillColourId, colors.sliderFill);
    hpSlider.setColour(juce::Slider::rotarySliderOutlineColourId, colors.sliderTrack);
    hpSlider.setColour(juce::Slider::trackColourId, colors.sliderTrack);
    hpSlider.setColour(juce::Slider::thumbColourId, colors.sliderThumb);

    lpSlider.setColour(juce::Slider::rotarySliderFillColourId, colors.sliderFill);
    lpSlider.setColour(juce::Slider::rotarySliderOutlineColourId, colors.sliderTrack);
    lpSlider.setColour(juce::Slider::trackColourId, colors.sliderTrack);
    lpSlider.setColour(juce::Slider::thumbColourId, colors.sliderThumb);

    bitSlider.setColour(juce::Slider::rotarySliderFillColourId, colors.sliderFill);
    bitSlider.setColour(juce::Slider::rotarySliderOutlineColourId, colors.sliderTrack);
    bitSlider.setColour(juce::Slider::trackColourId, colors.sliderTrack);
    bitSlider.setColour(juce::Slider::thumbColourId, colors.sliderThumb);

    rateSlider.setColour(juce::Slider::rotarySliderFillColourId, colors.sliderFill);
    rateSlider.setColour(juce::Slider::rotarySliderOutlineColourId, colors.sliderTrack);
    rateSlider.setColour(juce::Slider::trackColourId, colors.sliderTrack);
    rateSlider.setColour(juce::Slider::thumbColourId, colors.sliderThumb);

    // 2. Update the linear mix slider
    mixSlider.setColour(juce::Slider::trackColourId, colors.sliderFill);
    mixSlider.setColour(juce::Slider::backgroundColourId, colors.sliderTrack);
    mixSlider.setColour(juce::Slider::thumbColourId, colors.sliderThumb);

    // 3. Update all labels
    hpLabel.setColour(juce::Label::textColourId, colors.labelText);
    lpLabel.setColour(juce::Label::textColourId, colors.labelText);
    bitLabel.setColour(juce::Label::textColourId, colors.labelText);
    rateLabel.setColour(juce::Label::textColourId, colors.labelText);
    mixLabel.setColour(juce::Label::textColourId, colors.labelText);
    themeLabel.setColour(juce::Label::textColourId, colors.labelText);

    // 4. Update the gear icon color to match the text
    settingsButton.setColour(juce::TextButton::textColourOffId, colors.labelText);

    repaint(); 
}

// --- Rendering ---
void MyReduxEditor::paint(juce::Graphics &g) {
    auto colors = PluginTheme::getColors(currentThemeId);
    
    auto center = getLocalBounds().getCentre().toFloat();
    float radius = juce::jmax(getWidth(), getHeight()) * 0.7f;

    juce::ColourGradient gradient(colors.bgCenter, center.x, center.y, colors.bgEdge,
                                  center.x, center.y + radius, true);

    g.setGradientFill(gradient);
    g.fillAll();

    // Darken background if settings are open
    if (isSettingsVisible) {
        g.fillAll(juce::Colours::black.withAlpha(0.6f));
    }
}

void MyReduxEditor::resized() {
    auto fullBounds = getLocalBounds();
    
    // Gear Icon in the top right
    settingsButton.setBounds(fullBounds.getWidth() - 30, 5, 25, 25);

    // Settings overlay centered
    if (isSettingsVisible) {
        auto overlayArea = fullBounds.withSizeKeepingCentre(200, 80);
        themeLabel.setBounds(overlayArea.removeFromTop(30));
        themeSelector.setBounds(overlayArea.removeFromTop(30).reduced(10, 0));
        return; 
    }

    // --- Standard UI Layout ---
    auto bounds = fullBounds.reduced(20);
    const int gap = 15;

    // 1. Mix Section
    auto mixArea = bounds.removeFromRight(45);
    mixLabel.setBounds(mixArea.removeFromTop(25));
    mixSlider.setBounds(mixArea);

    bounds.removeFromRight(gap);

    // 2. Split vertical areas
    auto topArea = bounds.removeFromTop(bounds.getHeight() * 0.45f);
    bounds.removeFromTop(10); 

    // 3. Top Area: Filters
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

    // 4. Bottom Area: Main Controls
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