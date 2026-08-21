#include "ui/main/plugin_editor.h"
#include "dsp/plugin_processor.h"
// --- Main UI Controller File ---

// --- Constructor ---
MyPluginEditor::MyPluginEditor(MyPluginProcessor &p)
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
        bool isAnyMenuOpen = settingsOverlay.isVisible() || presetOverlay.isVisible();
        setLabelsVisible(!isAnyMenuOpen);
        repaint();
    };

    presetMenu.onPresetsToggled = [this](bool isOpen) {
        presetOverlay.setVisible(isOpen);
        if (isOpen) {
            pluginSettings.setMenuOpen(false);
            settingsOverlay.setVisible(false);
        }
        bool isAnyMenuOpen = settingsOverlay.isVisible() || presetOverlay.isVisible();
        setLabelsVisible(!isAnyMenuOpen);
        repaint();
    };

    pluginSettings.onFontSizeChanged = [this](int selectedId) {
        updateFontSize(selectedId);
        audioProcessor.saveFontSizeId(selectedId);
    };

    pluginSettings.onTooltipToggled = [this](bool isEnabled) {
        updateTooltipState(isEnabled);
        audioProcessor.saveTooltipState(isEnabled);
    };

    int initialTheme = audioProcessor.getSavedThemeId();
    pluginSettings.setInitialTheme(initialTheme);
    int initialFontSize = audioProcessor.getSavedFontSizeId();
    pluginSettings.setInitialFontSize(initialFontSize);
    currentFontSizeId = initialFontSize;
    setResizable(true, true);
    setResizeLimits(340, 340, 599, 599);
    getConstrainer()->setFixedAspectRatio(1.0);
    auto savedSize = audioProcessor.getWindowSize();
    setSize(savedSize.x, savedSize.y);
    bool initialTooltipState = audioProcessor.getSavedTooltipState();
    pluginSettings.setInitialTooltipState(initialTooltipState);
    updateTooltipState(initialTooltipState);
    updateTheme(initialTheme);
}

MyPluginEditor::~MyPluginEditor() {
    audioProcessor.saveWindowSize(getWidth(), getHeight());
    setLookAndFeel(nullptr);
}

void MyPluginEditor::updateTheme(int themeId) {
    currentThemeId = themeId;
    auto theme = PluginTheme::getThemeProps(themeId);
    themeLnF.currentFont = theme.labelFont.withHeight(getDynamicFontHeight());
    themeLnF.setColour(juce::ScrollBar::thumbColourId, theme.scrollbarThumb);

    // Standard Buttons
    themeLnF.setColour(juce::TextButton::buttonColourId, theme.buttonColor);
    themeLnF.setColour(juce::TextButton::buttonOnColourId, theme.buttonHoverColor);
    themeLnF.setColour(juce::TextButton::textColourOffId, theme.labelText);
    themeLnF.setColour(juce::TextButton::textColourOnId, theme.labelText);

    // Toggle Buttons / Checkboxes
    themeLnF.setColour(juce::ToggleButton::tickColourId, theme.buttonColor);
    themeLnF.setColour(juce::ToggleButton::tickDisabledColourId, theme.buttonColor.withAlpha(0.5f));
    themeLnF.setColour(juce::ToggleButton::textColourId, theme.labelText);

    // ComboBoxes (Dropdown Menus) & Popups
    themeLnF.setColour(juce::ComboBox::backgroundColourId, theme.bgCenter);
    themeLnF.setColour(juce::ComboBox::outlineColourId, theme.buttonHoverColor);
    themeLnF.setColour(juce::ComboBox::arrowColourId, theme.buttonHoverColor);
    themeLnF.setColour(juce::ComboBox::textColourId, theme.labelText);
    themeLnF.setColour(juce::PopupMenu::textColourId, theme.labelText);
    themeLnF.setColour(juce::PopupMenu::highlightedTextColourId, theme.bgCenter);
    themeLnF.setColour(juce::PopupMenu::backgroundColourId, theme.bgCenter);
    themeLnF.setColour(juce::PopupMenu::highlightedBackgroundColourId, theme.buttonHoverColor);
    themeLnF.setColour(juce::TooltipWindow::backgroundColourId, theme.bgCenter.withAlpha(0.8f));
    themeLnF.setColour(juce::TooltipWindow::textColourId, theme.labelText);
    themeLnF.setColour(juce::TooltipWindow::outlineColourId, theme.bgCenter.withAlpha(0.8f));

    sendLookAndFeelChange();

    auto applyRotaryTheme = [&](juce::Slider &slider) {
        slider.setColour(juce::Slider::rotarySliderFillColourId, theme.sliderFill);
        slider.setColour(juce::Slider::rotarySliderOutlineColourId, theme.sliderTrack);
        slider.setColour(juce::Slider::trackColourId, theme.sliderTrack);
        slider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);
        slider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);
    };

    auto applyLabelTheme = [&](juce::Label &label) { label.setColour(juce::Label::textColourId, theme.labelText); };

    // Rotary Sliders
    applyRotaryTheme(pluginControls.hpSlider);
    applyRotaryTheme(pluginControls.lpSlider);
    applyRotaryTheme(pluginControls.bitSlider);
    applyRotaryTheme(pluginControls.rateSlider);

    // Mix Slider
    pluginControls.mixSlider.setColour(juce::Slider::trackColourId, theme.sliderFill);
    pluginControls.mixSlider.setColour(juce::Slider::backgroundColourId, theme.sliderTrack);
    pluginControls.mixSlider.setColour(juce::Slider::thumbColourId, theme.sliderThumb);
    pluginControls.mixSlider.setColour(juce::Slider::textBoxTextColourId, theme.labelText);

    // Main UI Labels
    applyLabelTheme(pluginControls.hpLabel);
    applyLabelTheme(pluginControls.lpLabel);
    applyLabelTheme(pluginControls.bitLabel);
    applyLabelTheme(pluginControls.rateLabel);
    applyLabelTheme(pluginControls.mixLabel);

    // Overlay Labels
    applyLabelTheme(pluginSettings.themeLabel);
    applyLabelTheme(pluginSettings.fontSizeLabel);
    applyLabelTheme(pluginSettings.infoLabel);

    // Icons
    pluginSettings.updateIconColors(theme.buttonColor, theme.buttonHoverColor);
    presetMenu.updateIconColors(theme.buttonColor, theme.buttonHoverColor);

    // Menu Styling & Fonts
    pluginSettings.setThemeStyle(theme.labelText, themeLnF.currentFont);
    presetMenu.setThemeStyle(theme.labelText, themeLnF.currentFont);

    // Overlay Background Colors
    settingsOverlay.setOverlayColor(theme.setttingsOverlay);
    presetOverlay.setOverlayColor(theme.presetOverlay);

    // Refresh UI
    repaint();
}

void MyPluginEditor::paint(juce::Graphics &g) {
    auto ThemeProps = PluginTheme::getThemeProps(currentThemeId);
    auto center = getLocalBounds().getCentre().toFloat();
    float radius = juce::jmax(getWidth(), getHeight()) * 0.7f;
    juce::ColourGradient gradient(
        ThemeProps.bgCenter, center.x, center.y, ThemeProps.bgEdge, center.x, center.y + radius, true);
    g.setGradientFill(gradient);
    g.fillAll();
}

void MyPluginEditor::resized() {
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

void MyPluginEditor::updateFontSize(int fontSizeId) {
    currentFontSizeId = fontSizeId;
    updateTheme(currentThemeId);
}

float MyPluginEditor::getDynamicFontHeight() const {
    float baseFontHeight = juce::jmap<float>(static_cast<float>(getWidth()), 300.0f, 900.0f, 14.0f, 22.0f);
    float fontMultiplier = 1.1f;                        // Normal
    if (currentFontSizeId == 2) fontMultiplier = 0.9f;  // Small
    if (currentFontSizeId == 3) fontMultiplier = 1.2f;  // Large
    if (currentFontSizeId == 4) fontMultiplier = 1.27f; // Extra Large
    return baseFontHeight * fontMultiplier;
}

void MyPluginEditor::paintOverChildren(juce::Graphics &g) {
    bool isAnyMenuOpen = settingsOverlay.isVisible() || presetOverlay.isVisible();
    auto ThemeProps = PluginTheme::getThemeProps(currentThemeId);

    if (pluginControls.isVisible()) {
        float dynamicFontHeight = juce::jmap<float>(static_cast<float>(getWidth()), 300.0f, 900.0f, 14.0f, 22.0f);
        juce::Font logoFont = pluginControls.hpLabel.getLookAndFeel().getLabelFont(pluginControls.hpLabel);
        float logoScale = isAnyMenuOpen ? 1.5f : 2.5f;
        g.setFont(logoFont.withHeight(dynamicFontHeight * logoScale));
        juce::Rectangle<int> textBounds;
        if (isAnyMenuOpen) {
            int yOffset = (int)juce::jmap<float>(static_cast<float>(getHeight()), 340.0f, 600.0f, 15.0f, 28.0f);
            int yPosition = pluginControls.logoBounds.getY() - yOffset;
            textBounds = juce::Rectangle<int>(0, yPosition, getWidth(), pluginControls.logoBounds.getHeight());
        } else {
            int standardOffset = (int)juce::jmap<float>(static_cast<float>(getHeight()), 340.0f, 600.0f, 5.0f, 9.0f);
            textBounds = pluginControls.logoBounds.translated(7, -standardOffset);
        }
        g.setColour(ThemeProps.labelShadow);
        g.drawText("SimpleCrush", textBounds.translated(2, 2), juce::Justification::centred);
        g.setColour(ThemeProps.labelText);
        g.drawText("SimpleCrush", textBounds, juce::Justification::centred);
    }
}

void MyPluginEditor::setLabelsVisible(bool shouldBeVisible) {
    pluginControls.hpLabel.setVisible(shouldBeVisible);
    pluginControls.lpLabel.setVisible(shouldBeVisible);
    pluginControls.bitLabel.setVisible(shouldBeVisible);
    pluginControls.rateLabel.setVisible(shouldBeVisible);
    pluginControls.mixLabel.setVisible(shouldBeVisible);
}

void MyPluginEditor::updateTooltipState(bool shouldShowTooltips) {
    if (shouldShowTooltips) {
        if (tooltipWindow == nullptr) {
            tooltipWindow = std::make_unique<juce::TooltipWindow>(this, 1700);
            tooltipWindow->setOpaque(false);
        }
    } else {
        tooltipWindow.reset();
    }
}