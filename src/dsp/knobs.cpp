#include "knobs.h"

namespace Knobs {

void CustomKnob::mouseDown(const juce::MouseEvent &e) {
            juce::Slider::mouseDown(e);
            dragStartValue = getValue();
}

void CustomKnob::mouseDrag(const juce::MouseEvent &e) {
    if (isEnabled()) {
        if (e.mods.isShiftDown() || e.mods.isCtrlDown() || e.mods.isCommandDown()) {
            int deltaY = -e.getDistanceFromDragStartY();
            int deltaX = e.getDistanceFromDragStartX();
            int totalDeltaPixels = deltaY + deltaX;
            double stepSize = getInterval() > 0.0 ? getInterval() : 0.1;
            double sensitivity = 0.25;
            double absoluteOffset = totalDeltaPixels * stepSize * sensitivity;
            double newValue = dragStartValue + absoluteOffset;
            newValue = std::round(newValue / stepSize) * stepSize;
            newValue = juce::jlimit(getMinimum(), getMaximum(), newValue);
            setValue(newValue, juce::sendNotificationSync);
        } else {
            juce::Slider::mouseDrag(e);
            dragStartValue = getValue();
        }
    }
}

juce::NormalisableRange<float> makeCustomSkewRange(float start, float end, float skewFactor) {
    return juce::NormalisableRange<float>(start, end, 0.0f, skewFactor);
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // HPF - Custom Range
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"HPF", 1},
        "High Pass",
        makeCustomSkewRange(0.0f, 20000.0f, 0.3f),
        0.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 0) + " Hz"; },
        [](const juce::String &text) { return text.getFloatValue(); }));

    // BITS - Continuous interval (0.0f) manually passed in since there's no skew
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"BITS", 1},
        "Bit Depth",
        juce::NormalisableRange<float>(1.0f, 16.0f, 0.0f),
        16.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 1); },
        [](const juce::String &text) { return text.getFloatValue(); }));

    // RATE - Custom Range
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"RATE", 1},
        "Sample Rate",
        makeCustomSkewRange(1.0f, 44.1f, 0.6f),
        44.1f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 2); },
        [](const juce::String &text) { return text.getFloatValue(); }));

    // LPF - Custom Range
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"LPF", 1},
        "Low Pass",
        makeCustomSkewRange(20.0f, 20000.0f, 0.3f),
        20000.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 0) + " Hz"; },
        [](const juce::String &text) { return text.getFloatValue(); }));

    // MIX - Continuous interval (0.0f)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"MIX", 1},
        "Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.0f),
        1.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value * 100.0f, 1) + "%"; },
        [](const juce::String &text) { return text.getFloatValue() / 100.0f; }));

    layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"THEME_ID", 1}, "Theme ID", 1, 8, 1));

    return layout;
}

} // namespace Knobs