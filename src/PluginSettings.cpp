#include "PluginSettings.h"
#include "PluginTheme.h"
PluginSettings::PluginSettings() {
    juce::String infotext = "SimpleCrush v1.2.\n2026 Syverson Audio.\nAll rights reserved.\nDeveloped by Shea Syverson.";
    setInterceptsMouseClicks(false, true);

    // --- Settings Gear Button ---
    parseSvgIcon(settingsButton, drawableGear, drawableGearHover, SvgAssets::gearIcon);
    addAndMakeVisible(settingsButton);

    themeLabel.setText("Theme", juce::dontSendNotification);
    themeLabel.setJustificationType(juce::Justification::centred);
    addChildComponent(themeLabel);

    themeSelector.addItem("Panda Trueno", 1);
    themeSelector.addItem("Studio Dark", 2);
    themeSelector.addItem("Studio Light", 3);
    themeSelector.addItem("Vaporwave", 4);
    themeSelector.addItem("Retro Caramel", 5);
    themeSelector.addItem("Arctic Freeze", 6);
    themeSelector.addItem("Midnight Hacker", 7);
    addChildComponent(themeSelector);

    fontSizeLabel.setText("Font Size", juce::dontSendNotification);
    fontSizeLabel.setJustificationType(juce::Justification::centred);
    addChildComponent(fontSizeLabel);
    fontSizeSelector.addItem("Normal", 1);
    fontSizeSelector.addItem("Small", 2);
    fontSizeSelector.addItem("Large", 3);
    fontSizeSelector.addItem("Extra Large", 4);
    addChildComponent(fontSizeSelector);

    infoLabel.setText(infotext, juce::dontSendNotification);
    infoLabel.setJustificationType(juce::Justification::centred);
    addChildComponent(infoLabel);
    infoButton.onClick = [] { juce::URL("https://sheasyve.dev/simplecrush").launchInDefaultBrowser(); };
    infoButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    infoButton.setAlpha(0.0f);
    addChildComponent(infoButton);

    settingsButton.onClick = [this]() {
        isSettingsVisible = !isSettingsVisible;
        updateMenuVisibility();
    };

    themeSelector.onChange = [this]() {
        if (onThemeChanged != nullptr) onThemeChanged(themeSelector.getSelectedId());
    };

    fontSizeSelector.onChange = [this]() {
        if (onFontSizeChanged != nullptr) onFontSizeChanged(fontSizeSelector.getSelectedId());
    };
}

void PluginSettings::updateMenuVisibility() {
    // Settings Page
    themeLabel.setVisible(isSettingsVisible);
    themeSelector.setVisible(isSettingsVisible);
    fontSizeLabel.setVisible(isSettingsVisible);
    fontSizeSelector.setVisible(isSettingsVisible);
    infoLabel.setVisible(isSettingsVisible);
    infoButton.setVisible(isSettingsVisible);
    if (onSettingsToggled != nullptr) onSettingsToggled(isSettingsVisible);
    resized();
    repaint();
}

void PluginSettings::paint(juce::Graphics &g) {
    if (isSettingsVisible) {
        g.setColour(textColor);
        float titleSize = textFont.getHeight() * 1.2f;
        g.setFont(textFont.withHeight(titleSize).withStyle(juce::Font::bold));
        auto bounds = getLocalBounds();
        int textHeight = (int)titleSize + 10;
        int yPosition = (int)juce::jmap<float>(static_cast<float>(bounds.getHeight()), 340.0f, 600.0f, 40.0f, 79.0f);
        juce::Rectangle<int> titleArea(0, yPosition, bounds.getWidth(), textHeight);
        g.drawText("Settings", titleArea, juce::Justification::centred);
    }
}

void PluginSettings::resized() {
    auto bounds = getLocalBounds();
    settingsButton.setBounds(5, 5, 25, 25);
    if (isSettingsVisible) {
        int topY = bounds.getCentreY() - 110 + 7; 
        int availableHeight = bounds.getBottom() - topY - 10; 
        auto overlayArea = juce::Rectangle<int>(
            bounds.getCentreX() - 110, 
            topY, 
            220, 
            std::max(220, availableHeight) 
        );
        overlayArea.removeFromTop(5);
        themeLabel.setBounds(overlayArea.removeFromTop(40));
        themeSelector.setBounds(overlayArea.removeFromTop(25).reduced(10, 0));
        overlayArea.removeFromTop(15);
        fontSizeLabel.setBounds(overlayArea.removeFromTop(20));
        fontSizeSelector.setBounds(overlayArea.removeFromTop(25).reduced(10, 0));
        int labelHeight = 100; 
        auto centeredInfoBounds = overlayArea.withSizeKeepingCentre(overlayArea.getWidth(), labelHeight);
        infoLabel.setBounds(centeredInfoBounds);
        infoButton.setBounds(centeredInfoBounds);
    }
}

void PluginSettings::setMenuOpen(bool isOpen) {
    isSettingsVisible = isOpen;
    updateMenuVisibility();
}

void PluginSettings::updateIconColors(juce::Colour normal, juce::Colour hover) {
    if (drawableGear != nullptr && drawableGearHover != nullptr) {
        drawableGear->setFill(normal.brighter(0.3f));
        drawableGearHover->setFill(hover.brighter(0.3f));
        settingsButton.setImages(drawableGear.get(), drawableGearHover.get(), drawableGearHover.get());
    }
}

void PluginSettings::setInitialTheme(int themeId) { themeSelector.setSelectedId(themeId, juce::dontSendNotification); }

void PluginSettings::setInitialFontSize(int fontSizeId) {
    fontSizeSelector.setSelectedId(fontSizeId, juce::dontSendNotification);
}