cmake_minimum_required(VERSION 3.25)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)

project(SimpleCrush VERSION 1.0.1)

# Force MSVC to the absolute bleeding-edge standard
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/std:c++latest>")

cmake_policy(SET CMP0177 NEW)
add_subdirectory(JUCE)

juce_add_plugin(SimpleCrush
    COMPANY_NAME "Syverson Audio"
    BUNDLE_ID "com.SyversonAudio.SimpleCrush" # explicitly set to avoid spaces
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE "SyAu" 
    PLUGIN_CODE "Rdx4"
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "SimpleCrush"
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_COPY_DIR "C:/Users/sheam/Documents/VST" 
)

juce_generate_juce_header(SimpleCrush)

target_sources(SimpleCrush PRIVATE
    src/PluginProcessor.cpp
    src/PluginProcessor.h
    src/PluginEditor.cpp
    src/PluginEditor.h
    src/PluginTheme.h
    src/PluginControls.h
    src/PluginControls.cpp
    src/PluginSettings.h
    src/PluginSettings.cpp
    src/PresetMenu.h
    src/PresetMenu.cpp
    src/Util.h
)

target_link_libraries(SimpleCrush PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp)

target_compile_definitions(SimpleCrush PUBLIC JUCE_VST3_CAN_REPLACE_VST2=0)