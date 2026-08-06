#pragma once
#include <JuceHeader.h>

class AbsoluteFineTuneKnob : public juce::Slider {
public:
    AbsoluteFineTuneKnob() {
        setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    }

    void mouseDown(const juce::MouseEvent &e) override {
        juce::Slider::mouseDown(e);
        dragStartValue = getValue();
    }

    void mouseDrag(const juce::MouseEvent &e) override {
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

private:
    double dragStartValue = 0.0;
};