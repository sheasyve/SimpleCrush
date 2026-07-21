#include "PresetMenu.h"

// Helper lambda to parse SVGs cleanly without cluttering the constructor
auto parseSvgIcon = [](juce::DrawableButton& btn, std::unique_ptr<juce::DrawablePath>& normal, 
                       std::unique_ptr<juce::DrawablePath>& hover, const char* svgData) {
    juce::Path path;
    if (auto xml = juce::XmlDocument::parse(svgData)) {
        if (auto pathElement = xml->getChildByName("path"))
            path = juce::Drawable::parseSVGPath(pathElement->getStringAttribute("d"));
    }
    normal = std::make_unique<juce::DrawablePath>(); normal->setPath(path);
    hover = std::make_unique<juce::DrawablePath>(); hover->setPath(path);
    btn.setImages(normal.get(), hover.get(), hover.get());
};

PresetMenu::PresetMenu(juce::AudioProcessorValueTreeState& vts) : apvts(vts) {
    setInterceptsMouseClicks(false, true);

    // --- Preset Folder ---
    presetDirectory = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
        .getChildFile("SimpleCrush")
        .getChildFile("Presets");

    if (!presetDirectory.exists())
        presetDirectory.createDirectory();

    loadPresetsFromDirectory();

    // --- Presets List Button ---
    parseSvgIcon(presetsButton, drawableList, drawableListHover,
        "<svg viewBox='0 0 24 24'><path d='M3 13h2v-2H3v2zm0 4h2v-2H3v2zm0-8h2V7H3v2zm4 4h14v-2H7v2zm0 4h14v-2H7v2zM7 7v2h14V7H7z'/></svg>");
    addAndMakeVisible(presetsButton);

    // --- Preset Panel Title ---
    presetLabel.setText("PRESETS", juce::dontSendNotification);
    presetLabel.setJustificationType(juce::Justification::centred);
    addChildComponent(presetLabel);

    // --- Folder Button ---
    parseSvgIcon(folderButton, drawableFolder, drawableFolderHover,
        "<svg viewBox='0 0 24 24'><path d='M10 4H4c-1.1 0-1.99.9-1.99 2L2 18c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V8c0-1.1-.9-2-2-2h-8l-2-2z'/></svg>");
    addChildComponent(folderButton);

    // --- Save Text Box ---
    saveTextBox.setTextToShowWhenEmpty("New Preset Name...", juce::Colours::grey);
    saveTextBox.setMultiLine(false);
    saveTextBox.setReturnKeyStartsNewLine(false);
    addChildComponent(saveTextBox);

    // --- Save Button ---
    parseSvgIcon(saveButton, drawableSave, drawableSaveHover,
        "<svg viewBox='0 0 24 24'><path d='M17 3H5c-1.11 0-2 .9-2 2v14c0 1.1.89 2 2 2h14c1.1 0 2-.9 2-2V7l-4-4zm-5 16c-1.66 0-3-1.34-3-3s1.34-3 3-3 3 1.34 3 3-1.34 3-3 3zm3-10H5V5h10v4z'/></svg>");
    addChildComponent(saveButton);

    // --- Scrollable List Box ---
    presetList.setModel(this);
    presetList.setColour(juce::ListBox::backgroundColourId, juce::Colours::transparentBlack);
    addChildComponent(presetList);

    // --- Click Actions ---
    presetsButton.onClick = [this]() {
        isPresetsVisible = !isPresetsVisible;
        updateMenuVisibility();
        if (onPresetsToggled != nullptr)
            onPresetsToggled(isPresetsVisible);
    };

    folderButton.onClick = [this]() {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select Preset Folder", presetDirectory);
        
        auto browserFlags = juce::FileBrowserComponent::openMode 
                          | juce::FileBrowserComponent::canSelectDirectories;

        fileChooser->launchAsync(browserFlags, 
            [this](const juce::FileChooser& fc) {
                auto result = fc.getResult();
                if (result.exists()) {
                    presetDirectory = result;
                    loadPresetsFromDirectory();
                    juce::Logger::writeToLog("Folder selected: " + result.getFullPathName());
                }
            });
    };

    saveButton.onClick = [this]() {
        juce::String newPresetName = saveTextBox.getText();
        if (newPresetName.isNotEmpty()) {
            auto file = presetDirectory.getChildFile(newPresetName).withFileExtension(".preset");
            auto state = apvts.copyState();
            std::unique_ptr<juce::XmlElement> xml(state.createXml());
            if (xml != nullptr) {
                if (xml->writeTo(file)) {
                    loadPresetsFromDirectory();
                    saveTextBox.clear();
                }
            }
        }
    };
}

void PresetMenu::loadPresetsFromDirectory() {
    presets.clear();
    juce::Array<juce::File> results;
    presetDirectory.findChildFiles(results, juce::File::findFiles, false, "*.preset");
    for (auto& f : results) {
        presets.push_back({f.getFileNameWithoutExtension(), f});
    }
    
    presetList.updateContent();
}

void PresetMenu::updateMenuVisibility() {
    presetLabel.setVisible(isPresetsVisible);
    folderButton.setVisible(isPresetsVisible);
    saveTextBox.setVisible(isPresetsVisible);
    saveButton.setVisible(isPresetsVisible);
    presetList.setVisible(isPresetsVisible);
    resized();
    repaint();
}

void PresetMenu::setMenuOpen(bool isOpen) {
    isPresetsVisible = isOpen;
    updateMenuVisibility();
}

void PresetMenu::paint(juce::Graphics& g) {
    if (isPresetsVisible) {
        g.fillAll(juce::Colours::black.withAlpha(0.9f));
    }
}

void PresetMenu::resized() {
    auto bounds = getLocalBounds();
    presetsButton.setBounds(bounds.getWidth() - 30, 5, 25, 25); 
    if (isPresetsVisible) {
        auto overlayArea = bounds.withSizeKeepingCentre(260, 260); 
        presetLabel.setBounds(overlayArea.removeFromTop(30));
        auto topBar = overlayArea.removeFromTop(25);
        folderButton.setBounds(topBar.removeFromLeft(25).reduced(2));
        topBar.removeFromLeft(5); // gap
        saveButton.setBounds(topBar.removeFromRight(25).reduced(2));
        topBar.removeFromRight(5);
        saveTextBox.setBounds(topBar); 
        overlayArea.removeFromTop(10); 
        presetList.setBounds(overlayArea);
    }
}

void PresetMenu::updateIconColors(juce::Colour normal, juce::Colour hover) {
    if (drawableList != nullptr) {
        drawableList->setFill(normal);
        drawableListHover->setFill(hover);
        presetsButton.setImages(drawableList.get(), drawableListHover.get(), drawableListHover.get());
    }
    if (drawableFolder != nullptr) {
        drawableFolder->setFill(normal);
        drawableFolderHover->setFill(hover);
        folderButton.setImages(drawableFolder.get(), drawableFolderHover.get(), drawableFolderHover.get());
    }
    if (drawableSave != nullptr) {
        drawableSave->setFill(normal);
        drawableSaveHover->setFill(hover);
        saveButton.setImages(drawableSave.get(), drawableSaveHover.get(), drawableSaveHover.get());
    }
}

int PresetMenu::getNumRows() {
    return (int)presets.size();
}

void PresetMenu::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) {
    if (juce::isPositiveAndBelow(rowNumber, (int)presets.size())) {
        if (rowIsSelected) {
            g.fillAll(juce::Colours::white.withAlpha(0.2f)); // Highlight color
        }
        g.setColour(presetLabel.findColour(juce::Label::textColourId)); 
        g.setFont(juce::Font(14.0f));
        g.drawText(presets[rowNumber].name, 5, 0, width - 10, height, juce::Justification::centredLeft, true);
    }
}

void PresetMenu::listBoxItemClicked(int row, const juce::MouseEvent&) {
    if (juce::isPositiveAndBelow(row, (int)presets.size())) {
        auto file = presets[row].file;
        std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(file);
        
        if (xml != nullptr) {
            apvts.replaceState(juce::ValueTree::fromXml(*xml));
            juce::Logger::writeToLog("Loaded preset: " + presets[row].name);
        }
    }
}