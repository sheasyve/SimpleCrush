#include "preset_menu.h"

// --- PRESET MENU ---

PresetMenu::PresetMenu(juce::AudioProcessorValueTreeState &vts) : apvts(vts) {
    setInterceptsMouseClicks(false, true);

    // --- Preset Folder ---
    auto appDataDir =
        juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("SimpleCrush");
    presetDirectory = appDataDir.getChildFile("Presets");
    auto settingsFile = appDataDir.getChildFile("settings.xml");
    if (settingsFile.existsAsFile()) {
        if (std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(settingsFile)) {
            juce::String savedPath = xml->getStringAttribute("PresetFolder");
            if (savedPath.isNotEmpty()) {
                juce::File savedDir(savedPath);
                if (savedDir.exists() && savedDir.isDirectory()) { presetDirectory = savedDir; }
            }
        }
    }
    if (!presetDirectory.exists()) presetDirectory.createDirectory();
    loadPresetsFromDirectory();

    // --- Presets List Button ---
    parseSvgIcon(presetsButton, drawableList, drawableListHover, SvgAssets::listIcon);
    presetsButton.setTooltip(PresetTooltips::menuToggle);
    addAndMakeVisible(presetsButton);

    // --- Folder Button ---
    parseSvgIcon(folderButton, drawableFolder, drawableFolderHover, SvgAssets::folderIcon);
    folderButton.setTooltip(PresetTooltips::folder);
    addChildComponent(folderButton);

    // --- Delete Button ---
    parseSvgIcon(deleteButton, drawableDelete, drawableDeleteHover, SvgAssets::deleteIcon);
    deleteButton.setTooltip(PresetTooltips::deletePreset);
    addChildComponent(deleteButton);

    // --- Random Button ---
    parseSvgIcon(randomButton, drawableRandom, drawableRandomHover, SvgAssets::diceIcon);
    randomButton.setTooltip(PresetTooltips::randomize);
    addChildComponent(randomButton);

    // --- Save  ---
    saveTextBox.setTextToShowWhenEmpty("New Preset Name...", juce::Colours::grey);
    saveTextBox.setMultiLine(false);
    saveTextBox.setReturnKeyStartsNewLine(false);
    saveTextBox.setTooltip(PresetTooltips::saveInput);
    addChildComponent(saveTextBox);
    
    parseSvgIcon(saveButton, drawableSave, drawableSaveHover, SvgAssets::saveIcon);
    saveButton.setTooltip(PresetTooltips::saveButton);
    addChildComponent(saveButton);

    // --- Scrollable List Box ---
    presetList.setModel(this);
    presetList.setColour(juce::ListBox::backgroundColourId, juce::Colours::transparentBlack);
    addChildComponent(presetList);

    presetCallbacks();
}

void PresetMenu::loadPresetsFromDirectory() {
    presets.clear();
    juce::Array<juce::File> results;
    presetDirectory.findChildFiles(results, juce::File::findFiles, false, "*.preset");
    for (auto &f : results) { presets.push_back({f.getFileNameWithoutExtension(), f}); }
    presetList.updateContent();
}

void PresetMenu::updateMenuVisibility() {
    folderButton.setVisible(isPresetsVisible);
    deleteButton.setVisible(isPresetsVisible);
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
        float titleSize = textFont.getHeight() * 1.2f;
        g.setFont(textFont.withHeight(titleSize).withStyle(juce::Font::bold));
        auto bounds = getLocalBounds();
        int textHeight = (int)titleSize + 10;
        int yPosition = (int)juce::jmap<float>(static_cast<float>(bounds.getHeight()), 340.0f, 600.0f, 40.0f, 79.0f);
        juce::Rectangle<int> titleArea(0, yPosition, bounds.getWidth(), textHeight);
        g.drawText("Presets", titleArea, juce::Justification::centred);
    }
}

void PresetMenu::resized() {
    auto bounds = getLocalBounds();

    // --- Top Right Stack ---
    presetsButton.setBounds(bounds.getWidth() - 30, 5, 25, 25);
    saveButton.setBounds(bounds.getWidth() - 27.5, 30, 22.5, 22.5);
    randomButton.setBounds(bounds.getWidth() - 27.5, 55, 22.5, 22.5);

    // --- Top Left Stack ---
    folderButton.setBounds(5, 30, 25, 25);
    deleteButton.setBounds(5, 55, 25, 25);
    
    if (isPresetsVisible) {
        auto overlayArea = bounds.withSizeKeepingCentre(260, 240);
        overlayArea.translate(0, 25);
        auto topBar = overlayArea.removeFromTop(26);
        saveTextBox.setBounds(topBar.reduced(10, 0));
        overlayArea.removeFromTop(10);
        presetList.setBounds(overlayArea);
    }
}

void PresetMenu::updateIconColors(juce::Colour normal, juce::Colour hover) {
    juce::Colour brightNormal = normal.brighter(0.3f);
    juce::Colour brightHover = hover.brighter(0.3f);

    if (drawableList != nullptr) {
        drawableList->setFill(brightNormal);
        drawableListHover->setFill(brightHover);
        presetsButton.setImages(drawableList.get(), drawableListHover.get(), drawableListHover.get());
    }
    if (drawableFolder != nullptr) {
        drawableFolder->setFill(brightNormal);
        drawableFolderHover->setFill(brightHover);
        folderButton.setImages(drawableFolder.get(), drawableFolderHover.get(), drawableFolderHover.get());
    }
    if (drawableDelete != nullptr) {
        drawableDelete->setFill(brightNormal);
        drawableDeleteHover->setFill(brightHover);
        deleteButton.setImages(drawableDelete.get(), drawableDeleteHover.get(), drawableDeleteHover.get());
    }
    if (drawableSave != nullptr) {
        drawableSave->setFill(brightNormal);
        drawableSaveHover->setFill(brightHover);
        saveButton.setImages(drawableSave.get(), drawableSaveHover.get(), drawableSaveHover.get());
    }
    if (drawableRandom != nullptr) {
        drawableRandom->setFill(brightNormal);
        drawableRandomHover->setFill(brightHover);
        randomButton.setImages(drawableRandom.get(), drawableRandomHover.get(), drawableRandomHover.get());
    }
}

int PresetMenu::getNumRows() { return (int)presets.size(); }

void PresetMenu::paintListBoxItem(int rowNumber, juce::Graphics &g, int width, int height, bool rowIsSelected) {
    // --- The preset list ---
    if (juce::isPositiveAndBelow(rowNumber, (int)presets.size())) {
        if (rowIsSelected) { g.fillAll(juce::Colours::white.withAlpha(0.2f)); }
        g.setColour(textColor);
        g.setFont(textFont);
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
            saveTextBox.setText(presets[row].name, juce::dontSendNotification);
        }
    }
}