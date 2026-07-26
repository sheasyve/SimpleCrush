#include "PluginEditor.h"
#include "PluginProcessor.h"
// --- Main UI Controller File ---

// --- Constructor ---
MyReduxEditor::MyReduxEditor(MyReduxProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p), presetMenu(p.apvts), pluginControls(p.apvts) {
    setLookAndFeel(&themeLnF);
    addAndMakeVisible(pluginControls);
    addChildComponent(settingsOverlay);
    addChildComponent(presetOverlay);
    addAndMakeVisible(pluginSettings);
    addAndMakeVisible(presetMenu);

    pluginSettings.onThemeChanged = [this](int selectedId) {
        updateTheme(selectedId);
        audioProcessor.saveThemeId(selectedId);
    };

    pluginSettings.onSettingsToggled = [this](bool isOpen) {
        settingsOverlay.setVisible(isOpen);
        if (isOpen) {
            presetMenu.setMenuOpen(false);
            presetOverlay.setVisible(false);
        }
        repaint(); 
    };

    presetMenu.onPresetsToggled = [this](bool isOpen) {
        presetOverlay.setVisible(isOpen);
        if (isOpen) {
            pluginSettings.setMenuOpen(false);
            settingsOverlay.setVisible(false);
        }
        repaint(); 
    };
    
    pluginSettings.onFontSizeChanged = [this](int selectedId) {
        updateFontSize(selectedId);
        audioProcessor.saveFontSizeId(selectedId);
    };

    int initialTheme = audioProcessor.getSavedThemeId();
    pluginSettings.setInitialTheme(initialTheme);
    int initialFontSize = 1; 
    pluginSettings.setInitialFontSize(initialFontSize);
    currentFontSizeId = initialFontSize;
    setResizable(true, true);
    setResizeLimits(340, 340, 599, 599);
    getConstrainer()->setFixedAspectRatio(1.0);
    auto savedSize = audioProcessor.getWindowSize();
    setSize(savedSize.x, savedSize.y);
    updateTheme(initialTheme);
}

MyReduxEditor::~MyReduxEditor() {
    audioProcessor.saveWindowSize(getWidth(), getHeight());
    setLookAndFeel(nullptr);
}

void MyReduxEditor::updateTheme(int themeId) {
    currentThemeId = themeId;
    auto theme = PluginTheme::getThemeProps(themeId);
    themeLnF.currentFont = theme.labelFont.withHeight(getDynamicFontHeight());
    themeLnF.setColour(juce::ScrollBar::thumbColourId, theme.scrollbarThumb);
    sendLookAndFeelChange();

    // --- Apply Slider Graphics Colors ---
    pluginControls.hpSlider.setColour(juce::Slider::rotarySliderFillColourId, theme.sliderFill);
    pluginControls.hpSlider.setColour(juce::Slider::rotarySliderOutlineColourId, theme.sliderTrack);
    pluginControls.hpSlider.setColour(juce::Slider::trackColourId, theme.sliderTrack);
    pluginControls.hpSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);
    pluginControls.lpSlider.setColour(juce::Slider::rotarySliderFillColourId, theme.sliderFill);
    pluginControls.lpSlider.setColour(juce::Slider::rotarySliderOutlineColourId, theme.sliderTrack);
    pluginControls.lpSlider.setColour(juce::Slider::trackColourId, theme.sliderTrack);
    pluginControls.lpSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);
    pluginControls.bitSlider.setColour(juce::Slider::rotarySliderFillColourId, theme.sliderFill);
    pluginControls.bitSlider.setColour(juce::Slider::rotarySliderOutlineColourId, theme.sliderTrack);
    pluginControls.bitSlider.setColour(juce::Slider::trackColourId, theme.sliderTrack);
    pluginControls.bitSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);
    pluginControls.rateSlider.setColour(juce::Slider::rotarySliderFillColourId, theme.sliderFill);
    pluginControls.rateSlider.setColour(juce::Slider::rotarySliderOutlineColourId, theme.sliderTrack);
    pluginControls.rateSlider.setColour(juce::Slider::trackColourId, theme.sliderTrack);
    pluginControls.rateSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);
    pluginControls.mixSlider.setColour(juce::Slider::trackColourId, theme.sliderFill);
    pluginControls.mixSlider.setColour(juce::Slider::backgroundColourId, theme.sliderTrack);
    pluginControls.mixSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);

    // --- Apply Main UI Text Colors ---
    pluginControls.hpLabel.setColour(juce::Label::textColourId, theme.labelText);
    pluginControls.lpLabel.setColour(juce::Label::textColourId, theme.labelText);
    pluginControls.bitLabel.setColour(juce::Label::textColourId, theme.labelText);
    pluginControls.rateLabel.setColour(juce::Label::textColourId, theme.labelText);
    pluginControls.mixLabel.setColour(juce::Label::textColourId, theme.labelText);

    pluginControls.hpSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    pluginControls.lpSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    pluginControls.bitSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    pluginControls.rateSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    pluginControls.mixSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);

    // --- Apply Overlay Label Colors ---
    pluginSettings.themeLabel.setColour(juce::Label::textColourId, theme.labelText);
    pluginSettings.fontSizeLabel.setColour(juce::Label::textColourId, theme.labelText);
    pluginSettings.infoLabel.setColour(juce::Label::textColourId, theme.labelText);

    // --- Apply Settings Icon Colors ---
    pluginSettings.updateIconColors(theme.settings, theme.settingsHover);
    presetMenu.updateIconColors(theme.settings, theme.settingsHover);
    
    pluginSettings.setThemeStyle(theme.labelText, themeLnF.currentFont);
    presetMenu.setThemeStyle(theme.labelText, themeLnF.currentFont);

    settingsOverlay.setOverlayColor(theme.setttingsOverlay);
    presetOverlay.setOverlayColor(theme.presetOverlay);

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
}

void MyReduxEditor::resized() {
    auto theme = PluginTheme::getThemeProps(currentThemeId);
    float dynamicFontHeight = getDynamicFontHeight();
    themeLnF.currentFont = theme.labelFont.withHeight(dynamicFontHeight);
    sendLookAndFeelChange();
    auto fullBounds = getLocalBounds();
    settingsOverlay.setBounds(fullBounds);
    presetOverlay.setBounds(fullBounds);
    pluginControls.setBounds(fullBounds);
    pluginSettings.setBounds(fullBounds);
    presetMenu.setBounds(fullBounds);
}

void MyReduxEditor::updateFontSize(int fontSizeId) {
    currentFontSizeId = fontSizeId;
    updateTheme(currentThemeId); 
}

float MyReduxEditor::getDynamicFontHeight() const {
    float baseFontHeight = juce::jmap<float>(static_cast<float>(getWidth()), 300.0f, 900.0f, 14.0f, 22.0f);
    float fontMultiplier = 1.0f; // Normal
    if (currentFontSizeId == 2) fontMultiplier = 0.85f; // Small
    if (currentFontSizeId == 3) fontMultiplier = 1.15f; // Large
    if (currentFontSizeId == 4) fontMultiplier = 1.30f; // Extra Large
    return baseFontHeight * fontMultiplier;
}

void MyReduxEditor::paintOverChildren(juce::Graphics &g) {
    bool isAnyMenuOpen = settingsOverlay.isVisible() || presetOverlay.isVisible();

    if (pluginControls.isVisible()) {
        auto ThemeProps = PluginTheme::getThemeProps(currentThemeId);
        float dynamicFontHeight = getDynamicFontHeight();
        g.setColour(ThemeProps.labelText);
        juce::Font logoFont = pluginControls.hpLabel.getLookAndFeel().getLabelFont(pluginControls.hpLabel);
        float logoScale = isAnyMenuOpen ? 1.5f : 2.5f;
        g.setFont(logoFont.withHeight(dynamicFontHeight * logoScale));
        
        if (isAnyMenuOpen) {
            int yOffset = (int)juce::jmap<float>(static_cast<float>(getHeight()), 340.0f, 600.0f, 15.0f, 28.0f);
            int yPosition = pluginControls.logoBounds.getY() - yOffset;
            
            juce::Rectangle<int> centerBounds(0, yPosition, getWidth(), pluginControls.logoBounds.getHeight());
            g.drawText("SimpleCrush", centerBounds, juce::Justification::centred);
        } else {
            // Normal offset
            int standardOffset = (int)juce::jmap<float>(static_cast<float>(getHeight()), 340.0f, 600.0f, 5.0f, 9.0f);
            g.drawText("SimpleCrush", pluginControls.logoBounds.translated(7, -standardOffset), juce::Justification::centred);
        }
    }
}