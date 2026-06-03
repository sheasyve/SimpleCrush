cmake_minimum_required(VERSION 3.15)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)

project(ReduxCrush VERSION 1.0.1)
cmake_policy(SET CMP0177 NEW)
add_subdirectory(JUCE)

juce_add_plugin(ReduxCrush
    COMPANY_NAME "SheasAudio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE "ShAu" 
    PLUGIN_CODE "Rdx4"
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "ReduxCrush"
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_COPY_DIR "C:/Users/sheam/Documents/VST" 
)

juce_generate_juce_header(ReduxCrush)

target_sources(ReduxCrush PRIVATE
    src/PluginProcessor.cpp
    src/PluginProcessor.h
    src/PluginEditor.cpp
    src/PluginEditor.h)

target_link_libraries(ReduxCrush PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp)

# 3. Global Definitions
target_compile_definitions(ReduxCrush PUBLIC JUCE_VST3_CAN_REPLACE_VST2=0)