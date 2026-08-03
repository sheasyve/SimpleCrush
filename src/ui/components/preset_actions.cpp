#include "preset_menu.h"

void PresetMenu::presetCallbacks() {
    // --- Click actions in the preset menu ---

    presetsButton.onClick = [this]() {
        isPresetsVisible = !isPresetsVisible;
        updateMenuVisibility();
        if (onPresetsToggled != nullptr) onPresetsToggled(isPresetsVisible);
    };

    folderButton.onClick = [this]() {
        fileChooser = std::make_unique<juce::FileChooser>("Select Preset Folder", presetDirectory);
        auto browserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
        fileChooser->launchAsync(browserFlags, [this](const juce::FileChooser &fc) {
            auto result = fc.getResult();
            if (result.exists()) {
                presetDirectory = result;
                loadPresetsFromDirectory();
                auto appDataDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                                      .getChildFile("SimpleCrush");
                auto settingsFile = appDataDir.getChildFile("settings.xml");
                std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(settingsFile);
                if (xml == nullptr) { xml = std::make_unique<juce::XmlElement>("SimpleCrushSettings"); }
                xml->setAttribute("PresetFolder", presetDirectory.getFullPathName());
                xml->writeTo(settingsFile);
                juce::Logger::writeToLog("Folder selected and saved: " + result.getFullPathName());
            }
        });
    };

    deleteButton.onClick = [this]() {
        int selectedRow = presetList.getSelectedRow();
        if (juce::isPositiveAndBelow(selectedRow, (int)presets.size())) {
            auto file = presets[selectedRow].file;
            if (file.existsAsFile()) {
                file.deleteFile();
                loadPresetsFromDirectory();
                saveTextBox.setText("", juce::dontSendNotification);
                juce::Logger::writeToLog("Deleted preset: " + file.getFileName());
            }
        }
    };

    saveButton.onClick = [this]() {
        juce::String newPresetName = saveTextBox.getText();
        if (newPresetName.isNotEmpty()) {
            auto file = presetDirectory.getChildFile(newPresetName).withFileExtension(".preset");
            auto state = apvts.copyState();
            std::unique_ptr<juce::XmlElement> xml(state.createXml());
            if (xml != nullptr) {
                if (xml->writeTo(file)) { loadPresetsFromDirectory(); }
            }
        }
    };

    randomButton.onClick = [this]() {
        auto &rng = juce::Random::getSystemRandom();
        for (auto child : apvts.state) {
            if (child.hasProperty("id")) {
                juce::String paramID = child.getProperty("id").toString();
                if (auto *param = apvts.getParameter(paramID)) {
                    float randomNormalizedValue = rng.nextFloat();
                    param->setValueNotifyingHost(randomNormalizedValue);
                }
            }
        }
    };
}
