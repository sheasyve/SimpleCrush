cmake_minimum_required(VERSION 3.15)
project(Reduxerizer VERSION 1.0.0)
cmake_policy(SET CMP0177 NEW)
add_subdirectory(JUCE)

# Define plugin
juce_add_plugin(Reduxerizer
    COMPANY_NAME "Shea's-Plugin-Company"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE "SPUC"
    PLUGIN_CODE "Rdux"
    FORMATS VST3 AU
    PRODUCT_NAME "Reduxerizer")

target_sources(Reduxerizer PRIVATE
    src/PluginProcessor.cpp
    src/PluginProcessor.h
    src/PluginEditor.cpp
    src/PluginEditor.h)

# Link JUCE modules
target_link_libraries(Reduxerizer PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp)

set(ABLETON_VST3_PATH "C:\\Users\\sheam\\Documents\\VST")

install(TARGETS Reduxerizer
        BUNDLE DESTINATION ${ABLETON_VST3_PATH}
        LIBRARY DESTINATION ${ABLETON_VST3_PATH}
        RUNTIME DESTINATION ${ABLETON_VST3_PATH})