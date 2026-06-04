#include "PluginProcessor.h"
#include "PluginEditor.h"

//Plugin Logic

MyReduxProcessor::MyReduxProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts (*this, nullptr, "Parameters", createParameterLayout())
#else
     : apvts (*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    apvts.state = juce::ValueTree (juce::Identifier ("Parameters"));
}

MyReduxProcessor::~MyReduxProcessor() {}

juce::AudioProcessorEditor* MyReduxProcessor::createEditor(){
    return new MyReduxEditor (*this); 
}

bool MyReduxProcessor::hasEditor() const{
    return true; 
}

// Standard JUCE Boilerplate
const juce::String MyReduxProcessor::getName() const { return JucePlugin_Name; }
bool MyReduxProcessor::acceptsMidi() const { return false; }
bool MyReduxProcessor::producesMidi() const { return false; }
bool MyReduxProcessor::isMidiEffect() const { return false; }
double MyReduxProcessor::getTailLengthSeconds() const { return 0.0; }
int MyReduxProcessor::getNumPrograms() { return 1; }
int MyReduxProcessor::getCurrentProgram() { return 0; }
void MyReduxProcessor::setCurrentProgram (int index) {}
const juce::String MyReduxProcessor::getProgramName (int index) { return {}; }
void MyReduxProcessor::changeProgramName (int index, const juce::String& newName) {}
void MyReduxProcessor::releaseResources() {}

bool MyReduxProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const {
    auto mainOut = layouts.getMainOutputChannelSet();
    auto mainIn = layouts.getMainInputChannelSet();

    // Allow Mono or Stereo output
    if (mainOut != juce::AudioChannelSet::mono() && mainOut != juce::AudioChannelSet::stereo())
        return false;

    if (mainOut != mainIn)
        return false;

    return true;
}

// Save/Load State
void MyReduxProcessor::getStateInformation (juce::MemoryBlock& destData){
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    if (xml != nullptr)
        copyXmlToBinary (*xml, destData);
}

void MyReduxProcessor::setStateInformation (const void* data, int sizeInBytes){
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr) {
        if (xmlState->hasTagName (apvts.state.getType())) {
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

// JUCE Main Instance Creation
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){
    return new MyReduxProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout MyReduxProcessor::createParameterLayout(){
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"BITS", 1}, "Bit Depth", 1.0f, 16.0f, 16.0f));
        
    // Knob goes from 1.0 kHz to 44.1 kHz, defaulting to 44.1 kHz (Clean)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"RATE", 1}, "Sample Rate", 1.0f, 44.1f, 44.1f));

    return layout;
}

void MyReduxProcessor::prepareToPlay (double sampleRate, int samplesPerBlock){
    auto numChannels = getTotalNumInputChannels();
    heldSamples.assign(numChannels, 0.0f);
    sampleCounters.assign(numChannels, 0);
}

void MyReduxProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages){//The core logic of the redux effect.
    juce::ScopedNoDenormals noDenormals; 
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto actualBufferChannels   = buffer.getNumChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i){
        if (i < actualBufferChannels)
            buffer.clear (i, 0, buffer.getNumSamples());
    }

    // Get params from gui.
    auto* rawBits = apvts.getRawParameterValue("BITS");
    auto* rawRate = apvts.getRawParameterValue("RATE");

    // Safely load the float values.
    float bits = (rawBits != nullptr) ? rawBits->load() : 16.0f;
    float targetRateKHz = apvts.getRawParameterValue("RATE")->load();
    float targetRateHz = targetRateKHz * 1000.0f;
    int rate = std::max(1, static_cast<int>(getSampleRate() / targetRateHz));
    
    if (rate < 1) rate = 1;

    float totalLevels = std::pow(2.0f, bits);//2^16 = 65,536 levels. 2^4 = 16 levels.

    if (heldSamples.size() < static_cast<size_t>(totalNumInputChannels))
        heldSamples.resize(totalNumInputChannels, 0.0f);
        
    if (sampleCounters.size() < static_cast<size_t>(totalNumInputChannels))
        sampleCounters.resize(totalNumInputChannels, 0);

    int channelsToProcess = std::min(totalNumInputChannels, actualBufferChannels);

    for (int channel = 0; channel < channelsToProcess; ++channel){//THE CORE DSP LOOP (BITCRUSHING)
        auto* channelData = buffer.getWritePointer (channel);//The audio channel data.
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
            float inputSample = channelData[sample];// Read the current audio sample (a number between -1.0 and 1.0)
            // --- EFFECT 1: DOWNSAMPLING (Time Quantization) ---
            if (sampleCounters[channel] % rate == 0){//Only update the 'held' sample if our counter hits the rate divider.
                // --- EFFECT 2: BIT DEPTH REDUCTION (Amplitude Quantization) ---
                heldSamples[channel] = std::round(inputSample * totalLevels) / totalLevels;//Multiply the float by total steps, round it to the nearest whole step and divide.
            }
            channelData[sample] = heldSamples[channel];// Overwrite the original audio.
            sampleCounters[channel]++;
        }
    }
}