cmake_minimum_required(VERSION 3.22)
project(DrumAcademyVST VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point this to your JUCE checkout if JUCE isn't added as a submodule.
# Example: -DJUCE_DIR=/path/to/JUCE
if (DEFINED JUCE_DIR)
  add_subdirectory(${JUCE_DIR} JUCE)
else()
  message(FATAL_ERROR "Set JUCE_DIR to your JUCE path. Example: cmake -B build -DJUCE_DIR=~/JUCE")
endif()

juce_add_plugin(DrumAcademy
  COMPANY_NAME "Moise T. Sahouo"
  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  EDITOR_WANTS_KEYBOARD_FOCUS FALSE
  COPY_PLUGIN_AFTER_BUILD TRUE
  PLUGIN_MANUFACTURER_CODE MTSH
  PLUGIN_CODE DAKT
  FORMATS VST3 Standalone
  PRODUCT_NAME "Drum Academy"
)

target_sources(DrumAcademy PRIVATE
  source/PluginProcessor.cpp
  source/PluginProcessor.h
  source/PluginEditor.cpp
  source/PluginEditor.h
)

target_compile_definitions(DrumAcademy
  PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)

target_link_libraries(DrumAcademy
  PRIVATE
    juce::juce_audio_utils
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
