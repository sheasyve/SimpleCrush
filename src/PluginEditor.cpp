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
    juce::Colour settings;
    juce::Colour settingsHover;
};

inline Colors getColors(int themeId) {
    if (themeId == 1) { // Vaporwave
        return {juce::Colour(0xFFFF2A6D),
                juce::Colour(0xFF05D9E8),
                juce::Colour(0xFF01FFF4),
                juce::Colour(0xFF240046),
                juce::Colour(0xFF05D9E8),
                juce::Colour(0xFF10002B),
                juce::Colour(0xFF000000),
                juce::Colour(0xFFCCCCCC).withAlpha(0.3f),
                juce::Colour(0xFFCCCCCC).withAlpha(0.5f)};
    }

    if (themeId == 3) { // Midnight Hacker
        return {juce::Colour(0xFF00FF41), juce::Colour(0xFF008F11),
                juce::Colour(0xFFFFFFFF), juce::Colour(0xFF0D0208),
                juce::Colour(0xFF00FF41), juce::Colour(0xFF0D0208),
                juce::Colour(0xFF000000), juce::Colour(0xFF00FF41).withAlpha(0.3f),
                juce::Colour(0xFF00FF41)};
    }

    if (themeId == 4) { // Outrun Sunset
        return {juce::Colour(0xFFFF7E67), juce::Colour(0xFFF9C80E),
                juce::Colour(0xFFFFFFFF), juce::Colour(0xFF2A003F),
                juce::Colour(0xFFF9C80E), juce::Colour(0xFF2A003F),
                juce::Colour(0xFF0F0019), juce::Colour(0xFFFF7E67).withAlpha(0.3f),
                juce::Colour(0xFFF9C80E)};
    }

    if (themeId == 5) { // Arctic Freeze
        return {juce::Colour(0xFF74B3CE), juce::Colour(0xFF508991),
                juce::Colour(0xFFFFFFFF), juce::Colour(0xFF172A3A),
                juce::Colour(0xFF98C1D9), juce::Colour(0xFF172A3A),
                juce::Colour(0xFF09141C), juce::Colour(0xFF98C1D9).withAlpha(0.3f),
                juce::Colour(0xFFFFFFFF)};
    }

    // Default / Theme 2: Retro Caramel
    return {juce::Colour(0xFFAD7640),
            juce::Colour(0xFF6B4226),
            juce::Colour(0xFFC99966),
            juce::Colour(0xFF5C3822),
            juce::Colour(0xFFE3D3B5),
            juce::Colour(0xFF2E1A0F),
            juce::Colour(0xFF120804),
            juce::Colour(0xFFCCCCCC).withAlpha(0.3f),
            juce::Colour(0xFFCCCCCC).withAlpha(0.5f)};
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
    juce::Path gearPath;
    auto svg = juce::XmlDocument::parse(
        "<svg viewBox='0 0 24 24'>"
        "<path d='M19.43 "
        "12.98c.04-.32.07-.64.07-.98s-.03-.66-.07-.98l2.11-1.65c.19-.15.24-.42.12-.64l-2-3.46c-.12-"
        ".22-.39-.3-.61-.22l-2.49 1c-.52-.4-1.08-.73-1.69-.98l-.38-2.65C14.46 2.18 14.25 2 14 "
        "2h-4c-.25 0-.46.18-.49.42l-.38 2.65c-.61.25-1.17.59-1.69.98l-2.49-1c-.23-.09-.49 "
        "0-.61.22l-2 3.46c-.13.22-.07.49.12.64l2.11 1.65c-.04.32-.07.65-.07.98s.03.66.07.98l-2.11 "
        "1.65c-.19.15-.24.42-.12.64l2 3.46c.12.22.39.3.61.22l2.49-1c.52.4 1.08.73 1.69.98l.38 "
        "2.65c.03.24.24.42.49.42h4c.25 0 .46-.18.49-.42l.38-2.65c.61-.25 1.17-.59 1.69-.98l2.49 "
        "1c.23.09.49 0 .61-.22l2-3.46c.12-.22.07-.49-.12-.64l-2.11-1.65zM12 15.5c-1.93 "
        "0-3-1.57-3-3.5s1.07-3.5 3-3.5 3 1.57 3 3.5-1.07 3.5-3 3.5z'/>"
        "</svg>");

    if (svg != nullptr) {
        auto pathElement = svg->getChildByName("path");
        if (pathElement != nullptr)
            gearPath = juce::Drawable::parseSVGPath(pathElement->getStringAttribute("d"));
    }

    drawableGear = std::make_unique<juce::DrawablePath>();
    drawableGear->setPath(gearPath);
    drawableGearHover = std::make_unique<juce::DrawablePath>();
    drawableGearHover->setPath(gearPath);
    settingsButton.setImages(drawableGear.get(), drawableGearHover.get(), drawableGearHover.get());
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
        hpLabel.setVisible(!isSettingsVisible);
        bitLabel.setVisible(!isSettingsVisible);
        rateLabel.setVisible(!isSettingsVisible);
        lpLabel.setVisible(!isSettingsVisible);
        mixLabel.setVisible(!isSettingsVisible);
        resized();
        repaint();
    };

    // --- Theme Selector Overlay ---
    themeLabel.setText("THEME", juce::dontSendNotification);
    themeLabel.setJustificationType(juce::Justification::centred);
    addChildComponent(themeLabel);
    themeSelector.addItem("Vaporwave", 1);
    themeSelector.addItem("Retro Caramel", 2);
    themeSelector.addItem("Midnight Hacker", 3);
    themeSelector.addItem("Outrun Sunset", 4);
    themeSelector.addItem("Arctic Freeze", 5);
    themeSelector.setSelectedId(1, juce::dontSendNotification);
    addChildComponent(themeSelector);

    themeSelector.onChange = [this]() { updateTheme(themeSelector.getSelectedId()); };
    updateTheme(1);

    setSize(325, 300);
}

MyReduxEditor::~MyReduxEditor() {}

// --- Theme Update Logic ---
void MyReduxEditor::updateTheme(int themeId) {
    currentThemeId = themeId;
    auto colors = PluginTheme::getColors(themeId);

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
    mixSlider.setColour(juce::Slider::trackColourId, colors.sliderFill);
    mixSlider.setColour(juce::Slider::backgroundColourId, colors.sliderTrack);
    mixSlider.setColour(juce::Slider::thumbColourId, colors.sliderThumb);
    hpLabel.setColour(juce::Label::textColourId, colors.labelText);
    lpLabel.setColour(juce::Label::textColourId, colors.labelText);
    bitLabel.setColour(juce::Label::textColourId, colors.labelText);
    rateLabel.setColour(juce::Label::textColourId, colors.labelText);
    mixLabel.setColour(juce::Label::textColourId, colors.labelText);
    themeLabel.setColour(juce::Label::textColourId, colors.labelText);

    if (drawableGear != nullptr && drawableGearHover != nullptr) {
        drawableGear->setFill(colors.settings);
        drawableGearHover->setFill(colors.settingsHover);
        settingsButton.setImages(drawableGear.get(), drawableGearHover.get(),
                                 drawableGearHover.get());
    }
    repaint();
}

// --- Rendering ---
void MyReduxEditor::paint(juce::Graphics &g) {
    auto colors = PluginTheme::getColors(currentThemeId);
    auto center = getLocalBounds().getCentre().toFloat();
    float radius = juce::jmax(getWidth(), getHeight()) * 0.7f;
    juce::ColourGradient gradient(colors.bgCenter, center.x, center.y, colors.bgEdge, center.x,
                                  center.y + radius, true);
    g.setGradientFill(gradient);
    g.fillAll();
    if (isSettingsVisible) {
        g.fillAll(juce::Colours::black.withAlpha(0.6f));
    }
}

void MyReduxEditor::resized() {
    auto fullBounds = getLocalBounds();

    // Settings overlay
    settingsButton.setBounds(5, 5, 25, 25);
    if (isSettingsVisible) {
        auto overlayArea = fullBounds.withSizeKeepingCentre(200, 80);
        themeLabel.setBounds(overlayArea.removeFromTop(30));
        themeSelector.setBounds(overlayArea.removeFromTop(30).reduced(10, 0));
        return;
    }

    // Mix Section
    auto bounds = fullBounds.reduced(20);
    const int gap = 15;
    auto mixArea = bounds.removeFromRight(45);
    auto topArea = bounds.removeFromTop(bounds.getHeight() * 0.45f);
    mixLabel.setBounds(mixArea.removeFromTop(25));
    mixSlider.setBounds(mixArea);
    bounds.removeFromRight(gap);
    bounds.removeFromTop(10);

    // Top Area: Filters
    int filterKnobWidth = 70;
    int filterAreaWidth = (filterKnobWidth * 2) + gap;
    auto filterArea = topArea.withSizeKeepingCentre(filterAreaWidth, topArea.getHeight());
    auto hpArea = filterArea.removeFromLeft(filterKnobWidth);
    auto lpArea = filterArea.removeFromLeft(filterKnobWidth);
    filterArea.removeFromLeft(gap);
    hpLabel.setBounds(hpArea.removeFromTop(20));
    hpSlider.setBounds(hpArea);
    lpLabel.setBounds(lpArea.removeFromTop(20));
    lpSlider.setBounds(lpArea);

    // Bottom Area: Main Controls
    auto bottomArea = bounds;
    auto columnWidth = (bottomArea.getWidth() - gap) / 2;
    auto bitArea = bottomArea.removeFromLeft(columnWidth);
    auto rateArea = bottomArea.removeFromLeft(columnWidth);
    bottomArea.removeFromLeft(gap);
    bitLabel.setBounds(bitArea.removeFromTop(25));
    bitSlider.setBounds(bitArea);
    rateLabel.setBounds(rateArea.removeFromTop(25));
    rateSlider.setBounds(rateArea);
}