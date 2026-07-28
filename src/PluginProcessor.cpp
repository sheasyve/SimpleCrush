#include "PluginProcessor.h"
#include "PluginEditor.h"

// -- Core Processor File ---

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
    initPropertiesFile();

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

bool MyReduxProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
    auto mainOut = layouts.getMainOutputChannelSet();
    auto mainIn = layouts.getMainInputChannelSet();

    if (mainOut != juce::AudioChannelSet::mono() && mainOut != juce::AudioChannelSet::stereo())
        return false;

    if (mainOut != mainIn)
        return false;

    return true;
}

void MyReduxProcessor::getStateInformation(juce::MemoryBlock &destData) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    if (xml != nullptr)
        copyXmlToBinary(*xml, destData);
}

void MyReduxProcessor::setStateInformation(const void *data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr) {
        if (xmlState->hasTagName(apvts.state.getType())) {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new MyReduxProcessor(); }

juce::AudioProcessorValueTreeState::ParameterLayout MyReduxProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // --- High Pass (Skew: 0.3f) ---
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"HPF", 1},
        "High Pass",
        juce::NormalisableRange<float>(0.0f, 20000.0f, 1.0f, 0.3f),
        0.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 0) + " Hz"; },
        [](const juce::String &text) { return text.getFloatValue(); }));

    // --- Bit Depth (Linear) ---
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"BITS", 1},
        "Bit Depth",
        juce::NormalisableRange<float>(1.0f, 16.0f, 0.01f),
        16.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 1); },
        [](const juce::String &text) { return text.getFloatValue(); }));

    // --- Sample Rate (Skew: 0.4f) ---
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"RATE", 1},
        "Sample Rate",
        juce::NormalisableRange<float>(1.0f, 44.1f, 0.01f, 0.6f),
        44.1f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 2); },
        [](const juce::String &text) { return text.getFloatValue(); }));

    // --- Low Pass (Skew: 0.3f) ---
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"LPF", 1},
        "Low Pass",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
        20000.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 0) + " Hz"; },
        [](const juce::String &text) { return text.getFloatValue(); }));

    // --- Mix (Linear) ---
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"MIX", 1},
        "Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f),
        1.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value * 100.0f, 1) + "%"; },
        [](const juce::String &text) { return text.getFloatValue() / 100.0f; }));

    // --- Theme Selector ---
    layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"THEME_ID", 1}, "Theme ID", 1, 8, 1));

    return layout;
}

void MyReduxProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    auto numChannels = getTotalNumInputChannels();
    heldSamples.assign(numChannels, 0.0f);
    sampleCounters.assign(numChannels, 0);
    for (int i = 0; i < 2; ++i) {
        highPassFilters[i].reset();
        lowPassFilters[i].reset();
    }
}

void MyReduxProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {

    juce::ScopedNoDenormals noDenormals; // Prevents CPU spikes
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto actualBufferChannels = buffer.getNumChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        if (i < actualBufferChannels)
            buffer.clear(i, 0, buffer.getNumSamples());
    }

    // --- Parameter Retrieval & Setup ---
    auto *rawBits = apvts.getRawParameterValue("BITS");
    float bits = (rawBits != nullptr) ? rawBits->load() : 16.0f;
    float targetRateKHz = apvts.getRawParameterValue("RATE")->load();
    float targetRateHz = targetRateKHz * 1000.0f;
    int rate = std::max(1, static_cast<int>(getSampleRate() / targetRateHz));
    if (rate < 1)
        rate = 1;
    float totalLevels = std::pow(2.0f, bits);
    // Ensure state-tracking vectors are large enough for all active channels
    if (heldSamples.size() < static_cast<size_t>(totalNumInputChannels))
        heldSamples.resize(totalNumInputChannels, 0.0f);
    if (sampleCounters.size() < static_cast<size_t>(totalNumInputChannels))
        sampleCounters.resize(totalNumInputChannels, 0);

    int channelsToProcess = std::min(totalNumInputChannels, actualBufferChannels);
    float mix = apvts.getRawParameterValue("MIX")->load();
    // --- Update Filter Coefficients ---
    float sampleRate = getSampleRate();
    if (sampleRate > 0.0) {
        float hpFreq = apvts.getRawParameterValue("HPF")->load();
        float lpFreq = apvts.getRawParameterValue("LPF")->load();
        float maxFreq =
            sampleRate / 2.0f * 0.99f; // Prevent the low pass from exceeding the Nyquist limit (half the sample rate)
        auto hpCoeffs = juce::IIRCoefficients::makeHighPass(sampleRate, std::max(20.0f, hpFreq));
        auto lpCoeffs = juce::IIRCoefficients::makeLowPass(sampleRate, std::min(maxFreq, lpFreq));
        for (int i = 0; i < channelsToProcess; ++i) {
            if (i < 2) {
                highPassFilters[i].setCoefficients(hpCoeffs);
                lowPassFilters[i].setCoefficients(lpCoeffs);
            }
        }
    }
    // --- Main Audio Processing Loop ---
    for (int channel = 0; channel < channelsToProcess; ++channel) {
        auto *channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            float drySample = channelData[sample];
            if (sampleCounters[channel] % rate ==
                0) { // Sample and Hold: Only update held amplitude if the counter hits rate
                heldSamples[channel] =
                    std::round(drySample * totalLevels) / totalLevels;
            }
            float wetSample = heldSamples[channel];
            if (channel < 2) { // Apply filters to the crushed audio (only processing L and R channels)
                wetSample = highPassFilters[channel].processSingleSampleRaw(wetSample);
                wetSample = lowPassFilters[channel].processSingleSampleRaw(wetSample);
            }
            // Blend the dry and wet signals based on the mix knob position
            channelData[sample] = (drySample * (1.0f - mix)) + (wetSample * mix);
            sampleCounters[channel]++;
        }
    }
}

void MyReduxProcessor::initPropertiesFile() {
    juce::PropertiesFile::Options options;
    options.applicationName = "SimpleCrush";
    options.filenameSuffix = ".xml";
    options.folderName = "SimpleCrush";
    options.storageFormat = juce::PropertiesFile::storeAsXML;

    appProperties.setStorageParameters(options);
}

void MyReduxProcessor::saveWindowSize(int width, int height) {
    if (auto *props = appProperties.getUserSettings()) {
        props->setValue("WindowWidth", width);
        props->setValue("WindowHeight", height);
        props->saveIfNeeded();
    }
}

juce::Point<int> MyReduxProcessor::getWindowSize() {
    if (auto *props = appProperties.getUserSettings()) {
        int w = props->getIntValue("WindowWidth", 340);
        int h = props->getIntValue("WindowHeight", 340);
        return {w, h};
    }
    return {340, 340}; // Default fallback
}