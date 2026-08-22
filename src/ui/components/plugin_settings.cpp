#include "ui/components/plugin_settings.h"

PluginSettings::PluginSettings(PluginTheme::ThemeManager& tm) : themeManager(tm) {
    juce::String infotext =
        "SimpleCrush v1.2.\n2026 Syverson Audio.\nAll rights reserved.\nDeveloped by Shea Syverson.";
    setInterceptsMouseClicks(false, true);
    parseSvgIcon(settingsButton, drawableGear, drawableGearHover, SvgAssets::gearIcon);
    settingsButton.setTooltip(SettingsTooltips::settingsBtn);
    addAndMakeVisible(settingsButton);

    // Theme Setup
    themeLabel.setText("Theme", juce::dontSendNotification);
    themeLabel.setJustificationType(juce::Justification::centred);
    addChildComponent(themeLabel);

    themeSelector.setTooltip(SettingsTooltips::theme);
    addChildComponent(themeSelector);
    
    themeSelector.onChange = [this]() {
        int selectedId = themeSelector.getSelectedId();
        if (selectedId > 0 && onThemeChanged) {
            onThemeChanged(selectedId);
        }
    };

    // Load dynamic themes on start
    refreshThemeList();

    // Font Size Setup
    fontSizeLabel.setText("Font Size", juce::dontSendNotification);
    fontSizeLabel.setJustificationType(juce::Justification::centred);
    addChildComponent(fontSizeLabel);
    for (const auto &size : fontSizes) { fontSizeSelector.addItem(size.first, size.second); }
    fontSizeSelector.setTooltip(SettingsTooltips::fontSize);
    addChildComponent(fontSizeSelector);

    fontSizeSelector.onChange = [this]() {
        if (onFontSizeChanged != nullptr) onFontSizeChanged(fontSizeSelector.getSelectedId());
    };

    // Info Setup
    infoLabel.setText(infotext, juce::dontSendNotification);
    infoLabel.setJustificationType(juce::Justification::centred);
    addChildComponent(infoLabel);
    
    infoButton.onClick = [] { juce::URL("https://sheasyve.dev/simplecrush").launchInDefaultBrowser(); };
    infoButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    infoButton.setAlpha(0.0f);
    infoButton.setTooltip(SettingsTooltips::info);
    addChildComponent(infoButton);

    // Tooltip Toggle
    tooltipToggle.setTooltip(SettingsTooltips::tooltips);
    addChildComponent(tooltipToggle);
    tooltipToggle.onClick = [this]() {
        if (onTooltipToggled != nullptr) { onTooltipToggled(tooltipToggle.getToggleState()); }
    };

    // Settings Toggle Button
    settingsButton.onClick = [this]() {
        isSettingsVisible = !isSettingsVisible;
        updateMenuVisibility();
    };

    // Folder Button
    parseSvgIcon(folderButton, drawableFolder, drawableFolderHover, SvgAssets::folderIcon);
    folderButton.setTooltip("Move your Presets and Themes folder");
    addChildComponent(folderButton);
    
    folderButton.onClick = [this]() { launchFolderChooser(); };
}

void PluginSettings::updateMenuVisibility() {
    if (isSettingsVisible) refreshThemeList(); 
    
    // Settings Page
    themeLabel.setVisible(isSettingsVisible);
    themeSelector.setVisible(isSettingsVisible);
    fontSizeLabel.setVisible(isSettingsVisible);
    fontSizeSelector.setVisible(isSettingsVisible);
    infoLabel.setVisible(isSettingsVisible);
    infoButton.setVisible(isSettingsVisible);
    tooltipToggle.setVisible(isSettingsVisible);
    folderButton.setVisible(isSettingsVisible); 

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
        folderButton.setBounds(5, 30, 25, 25); 

        int topY = bounds.getCentreY() - 110 + 7;
        int labelHeight = 100;
        int availableHeight = bounds.getBottom() - topY - 10;
        auto overlayArea = juce::Rectangle<int>(bounds.getCentreX() - 110, topY, 220, std::max(220, availableHeight));
        
        overlayArea.removeFromTop(5);
        themeLabel.setBounds(overlayArea.removeFromTop(40));
        themeSelector.setBounds(overlayArea.removeFromTop(25).reduced(10, 0));
        overlayArea.removeFromTop(15);
        fontSizeLabel.setBounds(overlayArea.removeFromTop(20));
        fontSizeSelector.setBounds(overlayArea.removeFromTop(25).reduced(10, 0));
        overlayArea.removeFromTop(15);
        tooltipToggle.setBounds(overlayArea.removeFromTop(25).reduced(30, 0));
        
        auto centeredInfoBounds = overlayArea.withSizeKeepingCentre(overlayArea.getWidth(), labelHeight);
        infoLabel.setBounds(centeredInfoBounds);
        infoButton.setBounds(centeredInfoBounds);
    }
}

void PluginSettings::refreshThemeList() {
    themeSelector.clear(juce::dontSendNotification);
    auto availableThemes = themeManager.getAvailableThemes(); 
    
    for (const auto &[name, id] : availableThemes) {
        themeSelector.addItem(name, id);
    }
}

void PluginSettings::setInitialTheme(int themeId) { themeSelector.setSelectedId(themeId, juce::dontSendNotification); }

void PluginSettings::setMenuOpen(bool isOpen) {
    isSettingsVisible = isOpen;
    updateMenuVisibility();
}

void PluginSettings::updateIconColors(juce::Colour normal, juce::Colour hover) {
    juce::Colour brightNormal = normal.brighter(0.3f);
    juce::Colour brightHover = hover.brighter(0.3f);

    if (drawableGear != nullptr && drawableGearHover != nullptr) {
        drawableGear->setFill(brightNormal);
        drawableGearHover->setFill(brightHover);
        settingsButton.setImages(drawableGear.get(), drawableGearHover.get(), drawableGearHover.get());
    }
    if (drawableFolder != nullptr) {
        drawableFolder->setFill(brightNormal);
        drawableFolderHover->setFill(brightHover);
        folderButton.setImages(drawableFolder.get(), drawableFolderHover.get(), drawableFolderHover.get());
    }
}

void PluginSettings::setInitialFontSize(int fontSizeId) {
    fontSizeSelector.setSelectedId(fontSizeId, juce::dontSendNotification);
}

void PluginSettings::setInitialTooltipState(bool isEnabled) {
    tooltipToggle.setToggleState(isEnabled, juce::dontSendNotification);
}

void PluginSettings::launchFolderChooser() {
    fileChooser = std::make_unique<juce::FileChooser>(
        "Select new Data folder (containing Themes and Presets)...", 
        juce::File::getSpecialLocation(juce::File::userHomeDirectory), "");
    
    auto folderChooserFlags = juce::FileBrowserComponent::canSelectDirectories | 
                              juce::FileBrowserComponent::openMode;

    fileChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& fc) {
        auto newFolder = fc.getResult();
        if (newFolder.isDirectory()) {
            auto appDataDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("SimpleCrush");
            auto settingsFile = appDataDir.getChildFile("settings.xml");
            
            juce::XmlElement xml("SETTINGS");
            xml.setAttribute("PresetFolder", newFolder.getFullPathName()); 
            xml.writeTo(settingsFile);
            if (onDataFolderChanged) {
                onDataFolderChanged(newFolder);
            }
        }
    });
}