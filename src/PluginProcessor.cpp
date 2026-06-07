#include "PluginProcessor.h"
#include "PluginEditor.h"

// Plugin Logic
MyReduxProcessor::MyReduxProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
#else
    : apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    apvts.state = juce::ValueTree(juce::Identifier("Parameters"));
}

MyReduxProcessor::~MyReduxProcessor() {}

juce::AudioProcessorEditor *MyReduxProcessor::createEditor() { return new MyReduxEditor(*this); }
bool MyReduxProcessor::hasEditor() const { return true; }
const juce::String MyReduxProcessor::getName() const { return JucePlugin_Name; }
bool MyReduxProcessor::acceptsMidi() const { return false; }
bool MyReduxProcessor::producesMidi() const { return false; }
bool MyReduxProcessor::isMidiEffect() const { return false; }
double MyReduxProcessor::getTailLengthSeconds() const { return 0.0; }
int MyReduxProcessor::getNumPrograms() { return 1; }
int MyReduxProcessor::getCurrentProgram() { return 0; }
void MyReduxProcessor::setCurrentProgram(int index) {}
const juce::String MyReduxProcessor::getProgramName(int index) { return {}; }
void MyReduxProcessor::changeProgramName(int index, const juce::String &newName) {}
void MyReduxProcessor::releaseResources() {}

bool MyReduxProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    auto mainOut = layouts.getMainOutputChannelSet();
    auto mainIn = layouts.getMainInputChannelSet();

    if (mainOut != juce::AudioChannelSet::mono() && mainOut != juce::AudioChannelSet::stereo())
        return false;

    if (mainOut != mainIn)
        return false;

    return true;
}

void MyReduxProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    if (xml != nullptr)
        copyXmlToBinary(*xml, destData);
}

void MyReduxProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(apvts.state.getType()))
        {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new MyReduxProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout MyReduxProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"HPF", 1}, "High Pass", 
        juce::NormalisableRange<float>(0.0f, 20000.0f, 1.0f, 0.3f), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"BITS", 1}, "Bit Depth", 1.0f, 16.0f, 16.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"RATE", 1}, "Sample Rate", 1.0f, 44.1f, 44.1f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"LPF", 1}, "Low Pass", 
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f), 20000.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"MIX", 1}, "Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f, juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(juce::roundToInt(value * 100.0f)) + "%"; },
        [](const juce::String &text) { return text.getFloatValue() / 100.0f; }));

    return layout;
}

void MyReduxProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    auto numChannels = getTotalNumInputChannels();
    heldSamples.assign(numChannels, 0.0f);
    sampleCounters.assign(numChannels, 0);

    for (int i = 0; i < 2; ++i)
    {
        highPassFilters[i].reset();
        lowPassFilters[i].reset();
    }
}

void MyReduxProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto actualBufferChannels = buffer.getNumChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        if (i < actualBufferChannels)
            buffer.clear(i, 0, buffer.getNumSamples());
    }

    auto *rawBits = apvts.getRawParameterValue("BITS");
    float bits = (rawBits != nullptr) ? rawBits->load() : 16.0f;
    float targetRateKHz = apvts.getRawParameterValue("RATE")->load();
    float targetRateHz = targetRateKHz * 1000.0f;
    int rate = std::max(1, static_cast<int>(getSampleRate() / targetRateHz));

    if (rate < 1)
        rate = 1;

    float totalLevels = std::pow(2.0f, bits); 

    if (heldSamples.size() < static_cast<size_t>(totalNumInputChannels))
        heldSamples.resize(totalNumInputChannels, 0.0f);

    if (sampleCounters.size() < static_cast<size_t>(totalNumInputChannels))
        sampleCounters.resize(totalNumInputChannels, 0);

    int channelsToProcess = std::min(totalNumInputChannels, actualBufferChannels);
    float mix = apvts.getRawParameterValue("MIX")->load();

    float sampleRate = getSampleRate();
    if (sampleRate > 0.0)
    {
        float hpFreq = apvts.getRawParameterValue("HPF")->load();
        float lpFreq = apvts.getRawParameterValue("LPF")->load();
        float maxFreq = sampleRate / 2.0f * 0.99f; 

        auto hpCoeffs = juce::IIRCoefficients::makeHighPass(sampleRate, std::max(20.0f, hpFreq));
        auto lpCoeffs = juce::IIRCoefficients::makeLowPass(sampleRate, std::min(maxFreq, lpFreq));

        for (int i = 0; i < channelsToProcess; ++i)
        {
            if (i < 2) 
            {
                highPassFilters[i].setCoefficients(hpCoeffs);
                lowPassFilters[i].setCoefficients(lpCoeffs);
            }
        }
    }

    for (int channel = 0; channel < channelsToProcess; ++channel)
    { 
        auto *channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float drySample = channelData[sample];  
            
            if (sampleCounters[channel] % rate == 0)
            { 
                heldSamples[channel] = std::round(drySample * totalLevels) / totalLevels;
            }
            
            float wetSample = heldSamples[channel];

            if (channel < 2)
            {
                wetSample = highPassFilters[channel].processSingleSampleRaw(wetSample);
                wetSample = lowPassFilters[channel].processSingleSampleRaw(wetSample);
            }

            channelData[sample] = (drySample * (1.0f - mix)) + (wetSample * mix); 
            sampleCounters[channel]++;
        }
    }
}