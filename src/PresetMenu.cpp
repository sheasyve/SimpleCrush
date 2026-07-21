#include "PresetMenu.h"

PresetMenu::PresetMenu() {
    setInterceptsMouseClicks(false, true);

    // --- Presets List Button ---
    juce::Path listPath;
    auto listSvg = juce::XmlDocument::parse(
        "<svg viewBox='0 0 24 24'>"
        "<path d='M3 13h2v-2H3v2zm0 4h2v-2H3v2zm0-8h2V7H3v2zm4 4h14v-2H7v2zm0 4h14v-2H7v2zM7 7v2h14V7H7z'/>"
        "</svg>");

    if (listSvg != nullptr) {
        if (auto pathElement = listSvg->getChildByName("path"))
            listPath = juce::Drawable::parseSVGPath(pathElement->getStringAttribute("d"));
    }

    drawableList = std::make_unique<juce::DrawablePath>();
    drawableList->setPath(listPath);
    drawableListHover = std::make_unique<juce::DrawablePath>();
    drawableListHover->setPath(listPath);
    presetsButton.setImages(drawableList.get(), drawableListHover.get(), drawableListHover.get());
    addAndMakeVisible(presetsButton);

    // --- Preset Controls ---
    presetLabel.setText("PRESETS", juce::dontSendNotification);
    presetLabel.setJustificationType(juce::Justification::centred);
    addChildComponent(presetLabel);

    presetSelector.addItem("Init", 1);
    presetSelector.addItem("Lo-Fi Drums", 2);
    presetSelector.addItem("Vocal Choke", 3);
    addChildComponent(presetSelector);

    presetsButton.onClick = [this]() {
        isPresetsVisible = !isPresetsVisible;
        updateMenuVisibility();
        if (onPresetsToggled != nullptr)
            onPresetsToggled(isPresetsVisible);
    };
}

void PresetMenu::updateMenuVisibility() {
    presetLabel.setVisible(isPresetsVisible);
    presetSelector.setVisible(isPresetsVisible);
    resized();
    repaint();
}

void PresetMenu::setMenuOpen(bool isOpen) {
    isPresetsVisible = isOpen;
    updateMenuVisibility();
}

void PresetMenu::paint(juce::Graphics& g) {
    if (isPresetsVisible) {
        g.fillAll(juce::Colours::black.withAlpha(0.6f));
    }
}

void PresetMenu::resized() {
    auto bounds = getLocalBounds();

    presetsButton.setBounds(bounds.getWidth() - 30, 5, 25, 25); 

    if (isPresetsVisible) {
        auto overlayArea = bounds.withSizeKeepingCentre(220, 220);
        presetLabel.setBounds(overlayArea.removeFromTop(30));
        presetSelector.setBounds(overlayArea.removeFromTop(30).reduced(10, 0));
    }
}

void PresetMenu::updateIconColors(juce::Colour normal, juce::Colour hover) {
    if (drawableList != nullptr && drawableListHover != nullptr) {
        drawableList->setFill(normal);
        drawableListHover->setFill(hover);
        presetsButton.setImages(drawableList.get(), drawableListHover.get(), drawableListHover.get());
    }
}