#include "PluginEditor.h"
#include "PluginProcessor.h"

// --- Constructor ---
MyReduxEditor::MyReduxEditor(MyReduxProcessor &p) : AudioProcessorEditor(&p), audioProcessor(p) {
    setLookAndFeel(&themeLnF);
    // --- High Pass Knob ---
    hpSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    hpSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(hpSlider);
    hpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "HPF", hpSlider);
    hpLabel.setText("HIGH PASS", juce::dontSendNotification);
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
    themeSelector.addItem("Panda Trueno", 2);
    themeSelector.addItem("Studio Light", 3);
    themeSelector.addItem("Studio Dark", 4);
    themeSelector.addItem("Arctic Freeze", 5);
    themeSelector.addItem("Vintage Analog", 6);
    themeSelector.addItem("Retro Caramel", 7);
    themeSelector.addItem("Midnight Hacker", 8);
    themeSelector.setSelectedId(1, juce::dontSendNotification);
    addChildComponent(themeSelector);

    int initialTheme = audioProcessor.getSavedThemeId();
    themeSelector.setSelectedId(initialTheme, juce::dontSendNotification);
    themeSelector.onChange = [this]() { 
        int selectedId = themeSelector.getSelectedId();
        updateTheme(selectedId); 
        audioProcessor.saveThemeId(selectedId); 
    };
    updateTheme(initialTheme);

    setSize(300, 300);
}

MyReduxEditor::~MyReduxEditor() { setLookAndFeel(nullptr); }

void MyReduxEditor::updateTheme(int themeId) {
    currentThemeId = themeId;
    auto theme = PluginTheme::getThemeProps(themeId);

    themeLnF.currentFont = theme.labelFont;
    sendLookAndFeelChange();

    // --- 2. Apply Slider Graphics Colors ---
    hpSlider.setColour(juce::Slider::rotarySliderFillColourId, theme.sliderFill);
    hpSlider.setColour(juce::Slider::rotarySliderOutlineColourId, theme.sliderTrack);
    hpSlider.setColour(juce::Slider::trackColourId, theme.sliderTrack);
    hpSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);
    lpSlider.setColour(juce::Slider::rotarySliderFillColourId, theme.sliderFill);
    lpSlider.setColour(juce::Slider::rotarySliderOutlineColourId, theme.sliderTrack);
    lpSlider.setColour(juce::Slider::trackColourId, theme.sliderTrack);
    lpSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);
    bitSlider.setColour(juce::Slider::rotarySliderFillColourId, theme.sliderFill);
    bitSlider.setColour(juce::Slider::rotarySliderOutlineColourId, theme.sliderTrack);
    bitSlider.setColour(juce::Slider::trackColourId, theme.sliderTrack);
    bitSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);
    rateSlider.setColour(juce::Slider::rotarySliderFillColourId, theme.sliderFill);
    rateSlider.setColour(juce::Slider::rotarySliderOutlineColourId, theme.sliderTrack);
    rateSlider.setColour(juce::Slider::trackColourId, theme.sliderTrack);
    rateSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);
    mixSlider.setColour(juce::Slider::trackColourId, theme.sliderFill);
    mixSlider.setColour(juce::Slider::backgroundColourId, theme.sliderTrack);
    mixSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);

    // --- 3. Apply Text Colors ---
    hpLabel.setColour(juce::Label::textColourId, theme.labelText);
    lpLabel.setColour(juce::Label::textColourId, theme.labelText);
    bitLabel.setColour(juce::Label::textColourId, theme.labelText);
    rateLabel.setColour(juce::Label::textColourId, theme.labelText);
    mixLabel.setColour(juce::Label::textColourId, theme.labelText);
    themeLabel.setColour(juce::Label::textColourId, theme.labelText);
    hpSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    lpSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    bitSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    rateSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    mixSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);

    // --- 4. Apply Settings Icon Colors ---
    if (drawableGear != nullptr && drawableGearHover != nullptr) {
        drawableGear->setFill(theme.settings);
        drawableGearHover->setFill(theme.settingsHover);
        settingsButton.setImages(drawableGear.get(), drawableGearHover.get(),
                                 drawableGearHover.get());
    }

    repaint();
}
// --- Rendering ---
void MyReduxEditor::paint(juce::Graphics &g) {
    auto ThemeProps = PluginTheme::getThemeProps(currentThemeId);
    auto center = getLocalBounds().getCentre().toFloat();
    float radius = juce::jmax(getWidth(), getHeight()) * 0.7f;
    juce::ColourGradient gradient(ThemeProps.bgCenter, center.x, center.y, ThemeProps.bgEdge,
                                  center.x, center.y + radius, true);
    g.setGradientFill(gradient);
    g.fillAll();
    if (isSettingsVisible) {
        g.fillAll(juce::Colours::black.withAlpha(0.6f));
    }
}

void MyReduxEditor::resized() {
    //Plugin GUI Layout
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
    bounds.removeFromTop(10);

    // Top Area: Filters (Restored your smaller, centered logic!)
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