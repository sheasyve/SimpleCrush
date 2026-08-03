#pragma once
#include <JuceHeader.h>

class MyPluginProcessor : public juce::AudioProcessor {
public:
    MyPluginProcessor();
    ~MyPluginProcessor() override;

    // --- Core Audio Processing ---
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    // --- GUI Bridging ---
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    // --- Plugin Boilerplate ---
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{*this, nullptr, "Parameters", createParameterLayout()};
    void saveWindowSize(int width, int height);
    juce::Point<int> getWindowSize();
    int getSavedThemeId() { return appProperties.getUserSettings()->getIntValue("GlobalThemeId", 1); }
    void saveThemeId(int themeId) {
        appProperties.getUserSettings()->setValue("GlobalThemeId", themeId);
        appProperties.getUserSettings()->saveIfNeeded();
    }
    void saveFontSizeId(int fontSizeId);
    int getSavedFontSizeId();

    void saveTooltipState(bool isEnabled);
    bool getSavedTooltipState();

private:
    juce::ApplicationProperties appProperties;
    void initPropertiesFile();
    std::vector<float> heldSamples;
    std::vector<int> sampleCounters;
    juce::IIRFilter highPassFilters[2];
    juce::IIRFilter lowPassFilters[2];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyPluginProcessor)
};