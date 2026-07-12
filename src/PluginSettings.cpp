#include "PluginSettings.h"

PluginSettings::PluginSettings() {
    juce::String infotext = "SimpleCrush v1.1\n2026 Syverson Audio.\nAll rights reserved.\nDeveloped by Shea Syverson";
    setInterceptsMouseClicks(false, true);
    // --- Settings Gear Button ---
    juce::Path gearPath;
    auto svg = juce::XmlDocument::parse(
        "<svg viewBox='0 0 24 24'>"
        "<path d='M19.43 12.98c.04-.32.07-.64.07-.98s-.03-.66-.07-.98l2.11-1.65c.19-.15.24-.42.12-.64l-2-3.46c-.12-.22-.39-.3-.61-.22l-2.49 1c-.52-.4-1.08-.73-1.69-.98l-.38-2.65C14.46 2.18 14.25 2 14 2h-4c-.25 0-.46.18-.49.42l-.38 2.65c-.61.25-1.17.59-1.69.98l-2.49-1c-.23-.09-.49 0-.61.22l-2 3.46c-.13.22-.07.49.12.64l2.11 1.65c-.04.32-.07.65-.07.98s.03.66.07.98l-2.11 1.65c-.19.15-.24.42-.12.64l2 3.46c.12.22.39.3.61.22l2.49-1c.52.4 1.08.73 1.69.98l.38 2.65c.03.24.24.42.49.42h4c.25 0 .46-.18.49-.42l.38-2.65c.61-.25 1.17-.59 1.69-.98l2.49 1c.23.09.49 0 .61-.22l2-3.46c.12-.22.07-.49-.12-.64l-2.11-1.65zM12 15.5c-1.93 0-3-1.57-3-3.5s1.07-3.5 3-3.5 3 1.57 3 3.5-1.07 3.5-3 3.5z'/>"
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

    // --- Info Label ---
    infoLabel.setText(infotext, juce::dontSendNotification);
    infoLabel.setJustificationType(juce::Justification::centred);
    infoLabel.setMinimumHorizontalScale(1.0f);
    addChildComponent(infoLabel);

    linkButton.onClick = [] { juce::URL("https://sheasyve.dev/simplecrush").launchInDefaultBrowser(); };
    linkButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    linkButton.setAlpha(0.0f);
    addChildComponent(linkButton);

    // --- Theme Selector ---
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
    addChildComponent(themeSelector);

    // --- Button Actions ---
    settingsButton.onClick = [this]() {
        isSettingsVisible = !isSettingsVisible;
        
        themeSelector.setVisible(isSettingsVisible);
        themeLabel.setVisible(isSettingsVisible);
        infoLabel.setVisible(isSettingsVisible);
        linkButton.setVisible(isSettingsVisible);
        
        if (onSettingsToggled != nullptr)
            onSettingsToggled(isSettingsVisible);

        resized();
        repaint();
    };

    themeSelector.onChange = [this]() {
        if (onThemeChanged != nullptr)
            onThemeChanged(themeSelector.getSelectedId());
    };
}

void PluginSettings::setInitialTheme(int themeId) {
    themeSelector.setSelectedId(themeId, juce::dontSendNotification);
}

void PluginSettings::paint(juce::Graphics& g) {
    if (isSettingsVisible) {
        g.fillAll(juce::Colours::black.withAlpha(0.6f));
    }
}

void PluginSettings::resized() {
    auto bounds = getLocalBounds();
    settingsButton.setBounds(5, 5, 25, 25);

    if (isSettingsVisible) {
        auto overlayArea = bounds.withSizeKeepingCentre(220, 180);
        themeLabel.setBounds(overlayArea.removeFromTop(30));
        themeSelector.setBounds(overlayArea.removeFromTop(30).reduced(10, 0));
        overlayArea.removeFromTop(15);
        infoLabel.setBounds(overlayArea);
        linkButton.setBounds(overlayArea);
    }
}

void PluginSettings::updateIconColors(juce::Colour normal, juce::Colour hover) {
    if (drawableGear != nullptr && drawableGearHover != nullptr) {
        drawableGear->setFill(normal);
        drawableGearHover->setFill(hover);
        settingsButton.setImages(drawableGear.get(), drawableGearHover.get(), drawableGearHover.get());
    }
}