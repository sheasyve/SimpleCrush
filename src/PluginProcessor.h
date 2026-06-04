#pragma once
#include <JuceHeader.h>

class MyReduxProcessor : public juce::AudioProcessor
{
public:
    MyReduxProcessor();
    ~MyReduxProcessor() override;

    // --- Core Audio Processing ---
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    // --- GUI Bridging ---
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    // --- Standard Plugin Boilerplate ---
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

private:
    std::vector<float> heldSamples;
    std::vector<int> sampleCounters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyReduxProcessor)
};