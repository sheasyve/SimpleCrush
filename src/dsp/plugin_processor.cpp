#include "plugin_processor.h"
#include "ui/main/plugin_editor.h"

// --- CORE LIFECYCLE & ROUTING

MyPluginProcessor::MyPluginProcessor()
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

MyPluginProcessor::~MyPluginProcessor() {}

bool MyPluginProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
    auto mainOut = layouts.getMainOutputChannelSet();
    auto mainIn = layouts.getMainInputChannelSet();
    
    if (mainOut != juce::AudioChannelSet::mono() && mainOut != juce::AudioChannelSet::stereo()) return false;
    if (mainOut != mainIn) return false;
    
    return true;
}

// --- DSP SETUP ---

void MyPluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    auto numChannels = getTotalNumInputChannels();
    heldSamples.assign(numChannels, 0.0f);
    sampleCounters.assign(numChannels, 0);
    
    for (int i = 0; i < 2; ++i) {
        highPassFilters[i].reset();
        lowPassFilters[i].reset();
    }
}

void MyPluginProcessor::releaseResources() {}

// --- EDITOR & UI ---

bool MyPluginProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor *MyPluginProcessor::createEditor() { return new MyPluginEditor(*this); }

 
void MyPluginProcessor::getStateInformation(juce::MemoryBlock &destData) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    if (xml != nullptr) copyXmlToBinary(*xml, destData);
}

void MyPluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr) {
        if (xmlState->hasTagName(apvts.state.getType())) { 
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState)); 
        }
    }
}

// --- PARAMETER LAYOUT ---

juce::AudioProcessorValueTreeState::ParameterLayout MyPluginProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"HPF", 1}, "High Pass", juce::NormalisableRange<float>(0.0f, 20000.0f, 1.0f, 0.3f), 0.0f, juce::String(), juce::AudioProcessorParameter::genericParameter, [](float value, int) { return juce::String(value, 0) + " Hz"; }, [](const juce::String &text) { return text.getFloatValue(); }));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"BITS", 1}, "Bit Depth", juce::NormalisableRange<float>(1.0f, 16.0f, 0.01f), 16.0f, juce::String(), juce::AudioProcessorParameter::genericParameter, [](float value, int) { return juce::String(value, 1); }, [](const juce::String &text) { return text.getFloatValue(); }));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RATE", 1}, "Sample Rate", juce::NormalisableRange<float>(1.0f, 44.1f, 0.01f, 0.6f), 44.1f, juce::String(), juce::AudioProcessorParameter::genericParameter, [](float value, int) { return juce::String(value, 2); }, [](const juce::String &text) { return text.getFloatValue(); }));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LPF", 1}, "Low Pass", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f), 20000.0f, juce::String(), juce::AudioProcessorParameter::genericParameter, [](float value, int) { return juce::String(value, 0) + " Hz"; }, [](const juce::String &text) { return text.getFloatValue(); }));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"MIX", 1}, "Mix", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 1.0f, juce::String(), juce::AudioProcessorParameter::genericParameter, [](float value, int) { return juce::String(value * 100.0f, 1) + "%"; }, [](const juce::String &text) { return text.getFloatValue() / 100.0f; }));
    layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"THEME_ID", 1}, "Theme ID", 1, 8, 1));
    
    return layout;
}

// --- APP SETTINGS --- 

void MyPluginProcessor::initPropertiesFile() {
    juce::PropertiesFile::Options options;
    options.applicationName = "SimpleCrush";
    options.filenameSuffix  = ".xml";
    options.folderName      = "SimpleCrush";
    options.storageFormat   = juce::PropertiesFile::storeAsXML;
    appProperties.setStorageParameters(options);
}

void MyPluginProcessor::saveWindowSize(int width, int height) {
    if (auto *props = appProperties.getUserSettings()) {
        props->setValue("WindowWidth", width);
        props->setValue("WindowHeight", height);
        props->saveIfNeeded();
    }
}

juce::Point<int> MyPluginProcessor::getWindowSize() {
    if (auto *props = appProperties.getUserSettings()) {
        return { props->getIntValue("WindowWidth", 340), props->getIntValue("WindowHeight", 340) };
    }
    return {340, 340}; 
}

void MyPluginProcessor::saveFontSizeId(int fontSizeId) {
    if (auto *props = appProperties.getUserSettings()) {
        props->setValue("FontSizeId", fontSizeId);
        props->saveIfNeeded();
    }
}

int MyPluginProcessor::getSavedFontSizeId() {
    if (auto *props = appProperties.getUserSettings()) {
        return props->getIntValue("FontSizeId", 1); 
    }
    return 1;
}

// ==============================================================================
// JUCE BOILERPLATE
// ==============================================================================

const juce::String MyPluginProcessor::getName() const { return JucePlugin_Name; }
bool MyPluginProcessor::acceptsMidi() const { return false; }
bool MyPluginProcessor::producesMidi() const { return false; }
bool MyPluginProcessor::isMidiEffect() const { return false; }
double MyPluginProcessor::getTailLengthSeconds() const { return 0.0; }
int MyPluginProcessor::getNumPrograms() { return 1; }
int MyPluginProcessor::getCurrentProgram() { return 0; }
void MyPluginProcessor::setCurrentProgram(int index) {}
const juce::String MyPluginProcessor::getProgramName(int index) { return {}; }
void MyPluginProcessor::changeProgramName(int index, const juce::String &newName) {}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new MyPluginProcessor(); }