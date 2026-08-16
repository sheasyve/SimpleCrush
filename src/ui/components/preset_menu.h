#pragma once
#include "ui/style/plugin_theme.h"
#include "ui/style/tooltips.h"
#include "ui/style/svg.h"
#include <JuceHeader.h>

class PresetMenu : public juce::Component, public juce::ListBoxModel {
public:
    PresetMenu(juce::AudioProcessorValueTreeState &vts);
    ~PresetMenu() override = default;

    void paint(juce::Graphics &) override;
    void resized() override;
    void updateIconColors(juce::Colour normal, juce::Colour hover);
    void setMenuOpen(bool isOpen);
    bool isMenuOpen() const { return isPresetsVisible; }
    std::function<void(bool)> onPresetsToggled;
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics &g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent &) override;

    struct Preset {
        juce::String name;
        juce::File file;
    };

    void setThemeStyle(juce::Colour newColor, juce::Font newFont) {
        textColor = newColor;
        textFont = newFont;
        repaint();
    }

    juce::TextEditor saveTextBox;
    juce::ListBox presetList;

private:
    void updateMenuVisibility();
    void loadPresetsFromDirectory();
    void presetCallbacks();
    void SettingsFile(juce::File settingsFile);

    juce::AudioProcessorValueTreeState &apvts;
    juce::File presetDirectory;
    juce::Colour textColor = juce::Colours::white;
    juce::Font textFont;

    // --- Top Menu Buttons ---
    juce::DrawableButton presetsButton{"Presets", juce::DrawableButton::ImageFitted};
    std::unique_ptr<juce::DrawablePath> drawableList, drawableListHover;

    // --- Inner Preset Panel Buttons ---
    juce::DrawableButton folderButton{"Folder", juce::DrawableButton::ImageFitted};
    std::unique_ptr<juce::DrawablePath> drawableFolder, drawableFolderHover;
    juce::DrawableButton saveButton{"Save", juce::DrawableButton::ImageFitted};
    std::unique_ptr<juce::DrawablePath> drawableSave, drawableSaveHover;
    juce::DrawableButton deleteButton{"Delete", juce::DrawableButton::ImageFitted};
    std::unique_ptr<juce::DrawablePath> drawableDelete, drawableDeleteHover;
    juce::DrawableButton randomButton{"Random", juce::DrawableButton::ImageFitted};
    std::unique_ptr<juce::DrawablePath> drawableRandom, drawableRandomHover;

    // State & Data
    bool isPresetsVisible = false;
    std::vector<Preset> presets;
    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetMenu)
};