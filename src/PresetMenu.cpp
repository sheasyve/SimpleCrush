#include "PresetMenu.h"

PresetMenu::PresetMenu(juce::AudioProcessorValueTreeState &vts) : apvts(vts) {
    setInterceptsMouseClicks(false, true);

    // --- Preset Folder ---
    presetDirectory = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                      .getChildFile("SimpleCrush")
                      .getChildFile("Presets");

    if (!presetDirectory.exists())
        presetDirectory.createDirectory();
    loadPresetsFromDirectory();

    // --- Presets List Button ---
    parseSvgIcon(presetsButton, drawableList, drawableListHover, SvgAssets::listIcon);
    addAndMakeVisible(presetsButton);

    // --- Folder Button ---
    parseSvgIcon(folderButton, drawableFolder, drawableFolderHover, SvgAssets::folderIcon);
    addChildComponent(folderButton);

    // --- Random Button ---
    parseSvgIcon(randomButton, drawableRandom, drawableRandomHover, SvgAssets::diceIcon);
    addChildComponent(randomButton);

    // --- Save  ---
    saveTextBox.setTextToShowWhenEmpty("New Preset Name...", juce::Colours::grey);
    saveTextBox.setMultiLine(false);
    saveTextBox.setReturnKeyStartsNewLine(false);
    addChildComponent(saveTextBox);
    parseSvgIcon(saveButton, drawableSave, drawableSaveHover, SvgAssets::saveIcon);
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
        fileChooser = std::make_unique<juce::FileChooser>("Select Preset Folder", presetDirectory);
        auto browserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
        fileChooser->launchAsync(browserFlags, [this](const juce::FileChooser &fc) {
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

    randomButton.onClick = [this]() {
        if (presets.size() > 0) {
            int randomIndex = juce::Random::getSystemRandom().nextInt((int)presets.size());
            auto file = presets[randomIndex].file;
            std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(file);
            if (xml != nullptr) {
                apvts.replaceState(juce::ValueTree::fromXml(*xml));
                juce::Logger::writeToLog("Loaded random preset: " + presets[randomIndex].name);
                presetList.selectRow(randomIndex); 
            }
        }
    };
}

void PresetMenu::loadPresetsFromDirectory() {
    presets.clear();
    juce::Array<juce::File> results;
    presetDirectory.findChildFiles(results, juce::File::findFiles, false, "*.preset");
    for (auto &f : results) {
        presets.push_back({f.getFileNameWithoutExtension(), f});
    }
    presetList.updateContent();
}

void PresetMenu::updateMenuVisibility() {
    folderButton.setVisible(isPresetsVisible);
    saveTextBox.setVisible(isPresetsVisible);
    saveButton.setVisible(isPresetsVisible);
    randomButton.setVisible(isPresetsVisible); 
    presetList.setVisible(isPresetsVisible);

    resized();
    repaint();
}

void PresetMenu::setMenuOpen(bool isOpen) {
    isPresetsVisible = isOpen;
    updateMenuVisibility();
}

void PresetMenu::paint(juce::Graphics &g) {
    if (isPresetsVisible) {
        g.setColour(textColor);
        g.setFont(textFont.withHeight(32.0f).withStyle(juce::Font::bold));
        auto bounds = getLocalBounds();
        int textHeight = 40;
        int yPosition = 15;
        juce::Rectangle<int> titleArea(0, yPosition, bounds.getWidth(), textHeight);
        g.drawText("Presets", titleArea, juce::Justification::centred);
    }
}

void PresetMenu::resized() {
    auto bounds = getLocalBounds();
    presetsButton.setBounds(bounds.getWidth() - 30, 5, 25, 25);
    
    if (isPresetsVisible) {
        auto overlayArea = bounds.withSizeKeepingCentre(260, 240); 
        overlayArea.translate(0, 20);
        auto topBar = overlayArea.removeFromTop(25);
        // Left side
        folderButton.setBounds(topBar.removeFromLeft(25).reduced(2));
        topBar.removeFromLeft(5); // gap
        // Right side (Random button is furthest right)
        randomButton.setBounds(topBar.removeFromRight(25).reduced(2));
        topBar.removeFromRight(5); // gap between random and save
        saveButton.setBounds(topBar.removeFromRight(25).reduced(2));
        topBar.removeFromRight(5); // gap between save and textbox
        // Textbox takes remaining middle space
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
    
    if (drawableRandom != nullptr) {
        drawableRandom->setFill(normal);
        drawableRandomHover->setFill(hover);
        randomButton.setImages(drawableRandom.get(), drawableRandomHover.get(), drawableRandomHover.get());
    }
}

int PresetMenu::getNumRows() { return (int)presets.size(); }

void PresetMenu::paintListBoxItem(int rowNumber, juce::Graphics &g, int width, int height, bool rowIsSelected) {
    // --- The preset list ---
    if (juce::isPositiveAndBelow(rowNumber, (int)presets.size())) {
        if (rowIsSelected) {
            g.fillAll(juce::Colours::white.withAlpha(0.2f));
        }
        
        g.setColour(textColor);
        g.setFont(juce::Font(14.0f));
        g.drawText(presets[rowNumber].name, 5, 0, width - 10, height, juce::Justification::centredLeft, true);
    }
}

void PresetMenu::listBoxItemClicked(int row, const juce::MouseEvent &) {
    if (juce::isPositiveAndBelow(row, (int)presets.size())) {
        auto file = presets[row].file;
        std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(file);

        if (xml != nullptr) {
            apvts.replaceState(juce::ValueTree::fromXml(*xml));
            juce::Logger::writeToLog("Loaded preset: " + presets[row].name);
        }
    }
}

