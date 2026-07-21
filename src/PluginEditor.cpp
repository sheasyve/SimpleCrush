#include "PluginEditor.h"
#include "PluginProcessor.h"
// --- Main Gui Class ---

// --- Constructor ---
MyReduxEditor::MyReduxEditor(MyReduxProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p), pluginControls(p.apvts) {
    setLookAndFeel(&themeLnF);
    addAndMakeVisible(pluginControls);
    addAndMakeVisible(pluginSettings);
    addAndMakeVisible(presetMenu);
    pluginSettings.onThemeChanged = [this](int selectedId) {
        updateTheme(selectedId);
        audioProcessor.saveThemeId(selectedId);
    };
    pluginSettings.onSettingsToggled = [this](bool isVisible) { pluginControls.setVisible(!isVisible); };
    pluginSettings.onPresetsToggled = [this](bool isVisible) { pluginControls.setVisible(!isVisible); };
    int initialTheme = audioProcessor.getSavedThemeId();
    pluginSettings.setInitialTheme(initialTheme);
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
    float dynamicFontHeight = juce::jmap<float>(static_cast<float>(getWidth()), 300.0f, 900.0f, 14.0f, 22.0f);
    themeLnF.currentFont = theme.labelFont.withHeight(dynamicFontHeight);
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
    pluginSettings.presetLabel.setColour(juce::Label::textColourId, theme.labelText);
    pluginSettings.infoLabel.setColour(juce::Label::textColourId, theme.labelText);

    // --- Apply Settings Icon Colors ---
    pluginSettings.updateIconColors(theme.settings, theme.settingsHover);

    presetMenu.presetLabel.setColour(juce::Label::textColourId, theme.labelText);
    presetMenu.updateIconColors(theme.settings, theme.settingsHover);

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
    if (pluginControls.isVisible()) {
        float dynamicFontHeight = juce::jmap<float>(static_cast<float>(getWidth()), 300.0f, 900.0f, 14.0f, 22.0f);
        g.setColour(ThemeProps.labelText);
        juce::Font logoFont = pluginControls.hpLabel.getLookAndFeel().getLabelFont(pluginControls.hpLabel);
        g.setFont(logoFont.withHeight(dynamicFontHeight * 2.5f));
        g.drawText("SimpleCrush", pluginControls.logoBounds.translated(7, -5), juce::Justification::centred);
    }
}

void MyReduxEditor::resized() {
    auto theme = PluginTheme::getThemeProps(currentThemeId);
    float dynamicFontHeight = juce::jmap<float>(static_cast<float>(getWidth()), 300.0f, 900.0f, 14.0f, 22.0f);
    themeLnF.currentFont = theme.labelFont.withHeight(dynamicFontHeight);
    sendLookAndFeelChange();
    auto fullBounds = getLocalBounds();
    pluginControls.setBounds(fullBounds);
    pluginSettings.setBounds(fullBounds);
    presetMenu.setBounds(fullBounds);
}