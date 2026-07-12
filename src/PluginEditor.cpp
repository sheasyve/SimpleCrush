#include "PluginEditor.h"
#include "PluginProcessor.h"

// --- Constructor ---
MyReduxEditor::MyReduxEditor(MyReduxProcessor &p) : AudioProcessorEditor(&p), audioProcessor(p) {
    setLookAndFeel(&themeLnF);

    infoLabel.setText("SimpleCrush v1.1\n© 2026 Syverson Audio. All rights reserved.\nDeveloped by Shea Syverson", juce::dontSendNotification);

    // --- High Pass Knob ---
    hpSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    hpSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(hpSlider);
    hpAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "HPF", hpSlider);
    hpLabel.setText("HIGH PASS", juce::dontSendNotification);
    hpLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(hpLabel);

    // --- Bit Depth Knob ---
    bitSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bitSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    bitSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(bitSlider);
    bitAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "BITS", bitSlider);
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
    lpAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LPF", lpSlider);
    lpLabel.setText("LOW PASS", juce::dontSendNotification);
    lpLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lpLabel);

    // --- Mix Knob ---
    mixSlider.setSliderStyle(juce::Slider::LinearVertical);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(mixSlider);
    mixAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MIX", mixSlider);
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

    infoLabel.setText(infotext, juce::dontSendNotification);
    infoLabel.setJustificationType(juce::Justification::centred);
    infoLabel.setMinimumHorizontalScale(1.0f);
    addChildComponent(infoLabel);

    settingsButton.onClick = [this]() {
        isSettingsVisible = !isSettingsVisible;
        themeSelector.setVisible(isSettingsVisible);
        themeLabel.setVisible(isSettingsVisible);
        infoLabel.setVisible(isSettingsVisible);
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
    themeSelector.addItem("Retro Caramel", 5);
    themeSelector.addItem("Arctic Freeze", 6);
    themeSelector.addItem("Midnight Hacker", 7);
    themeSelector.setSelectedId(1, juce::dontSendNotification);
    addChildComponent(themeSelector);

    int initialTheme = audioProcessor.getSavedThemeId();
    themeSelector.setSelectedId(initialTheme, juce::dontSendNotification);
    themeSelector.onChange = [this]() {
        int selectedId = themeSelector.getSelectedId();
        updateTheme(selectedId);
        audioProcessor.saveThemeId(selectedId);
    };

    // --- Allow window resizing while keeping it a perfect square ---
    setResizable(true, true);
    setResizeLimits(340, 340, 599, 599);
    getConstrainer()->setFixedAspectRatio(1.0);

    // --- Apply initial size from settings file ---
    auto savedSize = audioProcessor.getWindowSize();
    setSize(savedSize.x, savedSize.y);

    // Call updateTheme after size is set so fonts are scaled correctly from launch
    updateTheme(initialTheme);
}

MyReduxEditor::~MyReduxEditor() {
    // --- Save Window Size When UI Closes ---
    audioProcessor.saveWindowSize(getWidth(), getHeight());
    setLookAndFeel(nullptr);
}

void MyReduxEditor::updateTheme(int themeId) {
    currentThemeId = themeId;
    auto theme = PluginTheme::getThemeProps(themeId);

    // --- Scale font smoothly using a bounded range ---
    float dynamicFontHeight = juce::jmap<float>(static_cast<float>(getWidth()), 300.0f, 900.0f, 14.0f, 22.0f);
    themeLnF.currentFont = theme.labelFont.withHeight(dynamicFontHeight);
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
    infoLabel.setColour(juce::Label::textColourId, theme.labelText);

    hpSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    lpSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    bitSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    rateSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    mixSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);

    // --- 4. Apply Settings Icon Colors ---
    if (drawableGear != nullptr && drawableGearHover != nullptr) {
        drawableGear->setFill(theme.settings);
        drawableGearHover->setFill(theme.settingsHover);
        settingsButton.setImages(drawableGear.get(), drawableGearHover.get(), drawableGearHover.get());
    }

    repaint();
}

void MyReduxEditor::paint(juce::Graphics &g) {
    auto ThemeProps = PluginTheme::getThemeProps(currentThemeId);
    auto center = getLocalBounds().getCentre().toFloat();
    float radius = juce::jmax(getWidth(), getHeight()) * 0.7f;
    juce::ColourGradient gradient(
        ThemeProps.bgCenter, center.x, center.y, ThemeProps.bgEdge, center.x, center.y + radius, true);
    
    g.setGradientFill(gradient);
    g.fillAll();

    if (!isSettingsVisible) {
        float dynamicFontHeight = juce::jmap<float>(static_cast<float>(getWidth()), 300.0f, 900.0f, 14.0f, 22.0f);
        g.setColour(ThemeProps.labelText);
        juce::Font logoFont = hpLabel.getLookAndFeel().getLabelFont(hpLabel);
        g.setFont(logoFont.withHeight(dynamicFontHeight * 2.5f));
        g.drawText("SimpleCrush", logoBounds.translated(7, -10), juce::Justification::centred);
    }

    if (isSettingsVisible) {
        g.fillAll(juce::Colours::black.withAlpha(0.6f));
    }
}

void MyReduxEditor::resized() {
    auto theme = PluginTheme::getThemeProps(currentThemeId);
    float dynamicFontHeight = juce::jmap<float>(static_cast<float>(getWidth()), 300.0f, 900.0f, 14.0f, 22.0f);
    themeLnF.currentFont = theme.labelFont.withHeight(dynamicFontHeight);
    sendLookAndFeelChange();

    // Plugin GUI Layout
    auto fullBounds = getLocalBounds();

    // Settings overlay
    settingsButton.setBounds(5, 5, 25, 25);
    if (isSettingsVisible) {
        auto overlayArea = fullBounds.withSizeKeepingCentre(220, 180);
        themeLabel.setBounds(overlayArea.removeFromTop(30));
        themeSelector.setBounds(overlayArea.removeFromTop(30).reduced(10, 0));

        overlayArea.removeFromTop(15);
        infoLabel.setBounds(overlayArea);
        return;
    }

    // Main Plugin Area
    auto bounds = fullBounds.reduced(fullBounds.getWidth() * 0.066f);
    const int gap = bounds.getWidth() * 0.05f;

    // Mix Section (Right Side)
    auto mixArea = bounds.removeFromRight(bounds.getWidth() * 0.17f);
    mixLabel.setBounds(mixArea.removeFromTop(30));
    mixSlider.setBounds(mixArea);

    // Gap between Mix slider and the rest of the knobs
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