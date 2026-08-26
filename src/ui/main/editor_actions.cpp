#include "ui/main/plugin_editor.h"
#include "dsp/plugin_processor.h"

void MyPluginEditor::setupCallbacks() {
    
    // --- Theme Changed ---
    pluginSettings.onThemeChanged = [this](int selectedId) {
        updateTheme(selectedId);
        audioProcessor.saveThemeId(selectedId);
    };

    // --- Settings Menu Toggled (Closes Presets if open) ---
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

    // --- Preset Menu Toggled (Closes Settings if open) ---
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

    // --- Font Size Changed ---
    pluginSettings.onFontSizeChanged = [this](int selectedId) {
        updateFontSize(selectedId);
        audioProcessor.saveFontSizeId(selectedId);
    };

    // --- Tooltip Toggled ---
    pluginSettings.onTooltipToggled = [this](bool isEnabled) {
        updateTooltipState(isEnabled);
        audioProcessor.saveTooltipState(isEnabled);
    };

    // --- Data Folder Changed ---
    pluginSettings.onDataFolderChanged = [this](const juce::File & /*newFolder*/) {
        auto liveSettingsFile = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                                    .getChildFile("Syverson Audio/SimpleCrush/settings.xml");
        presetMenu.SettingsFile(liveSettingsFile);
        presetMenu.loadPresetsFromDirectory();
        themeManager.loadSettings(liveSettingsFile);
        pluginSettings.refreshThemeList();
        
        updateTheme(currentThemeId);
    };

    // --- Folder Icon Clicked ---
    presetMenu.onFolderIconClicked = [this]() { 
        pluginSettings.launchFolderChooser(); 
    };
}