#include "PluginProcessor.h"
#include "PluginEditor.h"

// Constructor & Destructor
MyReduxProcessor::MyReduxProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif{
}

MyReduxProcessor::~MyReduxProcessor() {}

// Editor Creation
juce::AudioProcessorEditor* MyReduxProcessor::createEditor(){
    return new MyReduxEditor (*this); // Tells the DAW to open GUI
}

bool MyReduxProcessor::hasEditor() const{
    return true; 
}

// Standard JUCE Boilerplate
const juce::String MyReduxProcessor::getName() const { return JucePlugin_Name; }
bool MyReduxProcessor::acceptsMidi() const { return false; }
bool MyReduxProcessor::producesMidi() const { return false; }
bool MyReduxProcessor::isMidiEffect() const { return false; }
double MyReduxProcessor::getTailLength() const { return 0.0; }
int MyReduxProcessor::getNumPrograms() { return 1; }
int MyReduxProcessor::getCurrentProgram() { return 0; }
void MyReduxProcessor::setCurrentProgram (int index) {}
const juce::String MyReduxProcessor::getProgramName (int index) { return {}; }
void MyReduxProcessor::changeProgramName (int index, const juce::String& newName) {}
void MyReduxProcessor::releaseResources() {}
bool MyReduxProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const { return true; }

// Save/Load State
void MyReduxProcessor::getStateInformation (juce::MemoryBlock& destData){
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void MyReduxProcessor::setStateInformation (const void* data, int sizeInBytes){
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

// JUCE Main Instance Creation
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){
    return new MyReduxProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout MyReduxProcessor::createParameterLayout(){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    // "Bits" parameter: float from 1.0 to 16.0
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"BITS", 1}, "Bit Depth", 1.0f, 16.0f, 16.0f));
    // "Rate" parameter: integer divider from 1 to 32
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{"RATE", 1}, "Downsample Rate", 1, 32, 1));
    return { params.begin(), params.end() };
}

void MyReduxProcessor::prepareToPlay (double sampleRate, int samplesPerBlock){
    auto numChannels = getTotalNumInputChannels();
    heldSamples.assign(numChannels, 0.0f);
    sampleCounters.assign(numChannels, 0);
}

void MyReduxProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages){
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Load atomic parameter values from the UI/DAW thread
    float bits = apvts.getRawParameterValue("BITS")->load();
    int rate = static_cast<int>(apvts.getRawParameterValue("RATE")->load());

    // Calculate total discrete amplitude levels (e.g., 8-bit = 256 levels)
    float totalLevels = std::pow(2.0f, bits);

    for (int channel = 0; channel < totalNumInputChannels; ++channel){
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
            float inputSample = channelData[sample];

            // 1. Downsampling (Time Quantization / Zero-Order Hold)
            if (sampleCounters[channel] % rate == 0){
                // 2. Bitcrushing (Amplitude Quantization)
                heldSamples[channel] = std::round(inputSample * totalLevels) / totalLevels;
            }
            
            channelData[sample] = heldSamples[channel];
            sampleCounters[channel]++;
        }
    }
}