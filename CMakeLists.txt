cmake_minimum_required(VERSION 3.25)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)

project(SimpleCrush VERSION 1.0.1)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/std:c++latest>")

cmake_policy(SET CMP0177 NEW)
add_subdirectory(JUCE)

juce_add_plugin(SimpleCrush
    COMPANY_NAME "Syverson Audio"
    BUNDLE_ID "com.SyversonAudio.SimpleCrush"
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
    src/dsp/plugin_processor.cpp
    src/dsp/plugin_processor.h
    src/ui/main/plugin_editor.cpp
    src/ui/main/plugin_editor.h
    src/ui/main/plugin_controls.h
    src/ui/main/plugin_controls.cpp
    src/ui/style/plugin_theme.h
    src/ui/style/svg.h
    src/ui/components/plugin_settings.h
    src/ui/components/plugin_settings.cpp
    src/ui/components/preset_menu.h
    src/ui/components/preset_menu.cpp
)

target_link_libraries(SimpleCrush PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp)
    
target_include_directories(SimpleCrush PRIVATE src)
target_compile_definitions(SimpleCrush PUBLIC JUCE_VST3_CAN_REPLACE_VST2=0)