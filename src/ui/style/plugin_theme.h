#pragma once
#include <JuceHeader.h>

namespace PluginTheme {

struct ThemeProps {
    juce::Colour sliderFill;
    juce::Colour sliderOutline;
    juce::Colour sliderThumb;
    juce::Colour sliderTrack;
    juce::Colour labelText;
    juce::Colour labelShadow;
    juce::Colour bgCenter;
    juce::Colour bgEdge;
    juce::Colour buttonColor;
    juce::Colour buttonHoverColor;
    juce::FontOptions labelFont;
    juce::Colour setttingsOverlay;
    juce::Colour presetOverlay;
    juce::Colour scrollbarThumb;
};

struct SettingsBackground : public juce::Component {
    juce::Colour currentOverlayColor = juce::Colours::black.withAlpha(0.85f);
    void setOverlayColor(juce::Colour newColor) {
        currentOverlayColor = newColor;
        repaint();
    }
    void paint(juce::Graphics &g) override { g.fillAll(currentOverlayColor); }
};

inline ThemeProps getThemeProps(int themeId) {

    if (themeId == 1) { // Panda Trueno
        return {juce::Colour(0xFFFFFFFF), // sliderFill
            juce::Colour(0xFF1A1A1A),     // sliderOutline
            juce::Colour(0xFFE50000),     // sliderThumb
            juce::Colour(0xFF333333),     // sliderTrack
            juce::Colour(0xFFFFFFFF),     // labelText
            juce::Colour(0x7e2b2b2b),     // labelShadow
            juce::Colour(0xFF2B2B2B),     // bgCenter
            juce::Colour(0xFF050505),     // bgEdge
            juce::Colour(0xbdffffff),     // buttonColor
            juce::Colour(0xFFE50000),     // buttonHoverColor
            juce::FontOptions(15.0f, juce::Font::bold | juce::Font::italic).withName("Arial"), // labelFont
            juce::Colours::black.withAlpha(0.90f), // setttingsOverlay
            juce::Colours::black.withAlpha(0.8f),  // presetOverlay
            juce::Colour(0xFFE50000)};             // scrollbarThumb
    }

    if (themeId == 2) { // Studio Dark
        return {juce::Colour(0xffd6d6d6), // sliderFill
            juce::Colour(0xFF4C4C4C),     // sliderOutline
            juce::Colour(0xFFD4D4D4),     // sliderThumb
            juce::Colour(0xff575757),     // sliderTrack
            juce::Colour(0xfff1f1f1),     // labelText
            juce::Colour(0x81575757),     // labelShadow
            juce::Colour(0xFF202020),     // bgCenter
            juce::Colour(0xff131313),     // bgEdge
            juce::Colour(0xdea1a1a1),     // buttonColor
            juce::Colour(0xffebebeb),     // buttonHoverColor
            juce::FontOptions(15.0f, juce::Font::bold).withName("Arial"), // labelFont
            juce::Colours::black.withAlpha(0.9f),  // setttingsOverlay
            juce::Colours::black.withAlpha(0.8f),  // presetOverlay
            juce::Colour(0xFFD4D4D4)};             // scrollbarThumb
    }

    if (themeId == 3) { // Studio Light
        return {juce::Colour(0xFF333333), // sliderFill
            juce::Colour(0xFFAAAAAA),     // sliderOutline
            juce::Colour(0xFF333333),     // sliderThumb
            juce::Colour(0xffbbbbbb),     // sliderTrack
            juce::Colour(0xFF222222),     // labelText
            juce::Colour(0x83f9f9f9),     // labelShadow
            juce::Colour(0xFFF9F9F9),     // bgCenter
            juce::Colour(0xFFDCDCDC),     // bgEdge
            juce::Colour(0xff080808),     // buttonColor
            juce::Colour(0xff8d8d8d),     // buttonHoverColor
            juce::FontOptions(15.0f, juce::Font::bold).withName("Arial"), // labelFont
            juce::Colour(0xFFE0E0E0).withAlpha(0.83f), // setttingsOverlay
            juce::Colour(0xFFE0E0E0).withAlpha(0.83f), // presetOverlay
            juce::Colour(0xFF333333)};                 // scrollbarThumb
    }

    if (themeId == 4) { // Vaporwave
        return {juce::Colour(0xFFFF2A6D), // sliderFill
            juce::Colour(0xFF05D9E8),     // sliderOutline
            juce::Colour(0xFF01FFF4),     // sliderThumb
            juce::Colour(0xff14505f),     // sliderTrack
            juce::Colour(0xFF05D9E8),     // labelText
            juce::Colour(0x2aff2a6e),     // labelShadow
            juce::Colour(0xFF390B8A),     // bgCenter
            juce::Colour(0xFF000000),     // bgEdge
            juce::Colour(0xFF05D9E8),     // buttonColor
            juce::Colour(0xFFFF2A6D),     // buttonHoverColor
            juce::FontOptions(15.0f, juce::Font::bold | juce::Font::italic).withName("Arial"), // labelFont
            juce::Colours::black.withAlpha(0.8f),    // setttingsOverlay
            juce::Colours::black.withAlpha(0.78f),   // presetOverlay
            juce::Colour(0xFF01FFF4)};               // scrollbarThumb
    }

    if (themeId == 5) { // Retro Caramel
        return {juce::Colour(0xFFC58B4E), // sliderFill
            juce::Colour(0xFF9A623B),     // sliderOutline
            juce::Colour(0xFFDFB382),     // sliderThumb
            juce::Colour(0xFF7B4A2E),     // sliderTrack
            juce::Colour(0xFFF0E3CB),     // labelText
            juce::Colour(0x38dfb482),     // labelShadow
            juce::Colour(0xff644630),     // bgCenter
            juce::Colour(0xFF2B1910),     // bgEdge
            juce::Colour(0xFFDFB382),     // buttonColor
            juce::Colour(0xFF8C6C4B),     // buttonHoverColor
            juce::FontOptions(15.0f, juce::Font::italic | juce::Font::bold).withName("Helvetica"), // labelFont
            juce::Colour(0xFF2B1910).withAlpha(0.9f), // setttingsOverlay
            juce::Colour(0xFF2B1910).withAlpha(0.8f), // presetOverlay
            juce::Colour(0xFFDFB382)};                // scrollbarThumb
    }

    if (themeId == 6) { // Arctic Freeze
        return {juce::Colour(0xFF30C5FF), // sliderFill
            juce::Colour(0xFF0096C7),     // sliderOutline
            juce::Colour(0xFFFFFFFF),     // sliderThumb
            juce::Colour(0xFF0A2440),     // sliderTrack
            juce::Colour(0xFFADE8F4),     // labelText
            juce::Colour(0x5b0a2440),     // labelShadow
            juce::Colour(0xFF0A2440),     // bgCenter
            juce::Colour(0xFF031020),     // bgEdge
            juce::Colour(0xFF0096C7),     // buttonColor
            juce::Colour(0xFF30C5FF),     // buttonHoverColor
            juce::FontOptions(16.0f, juce::Font::bold).withName("Trebuchet MS"), // labelFont
            juce::Colour(0xFF031020).withAlpha(0.95f), // setttingsOverlay
            juce::Colour(0xFF031020).withAlpha(0.8f),  // presetOverlay
            juce::Colour(0xFFFFFFFF)};                 // scrollbarThumb
    }

    // Midnight Hacker
    return {juce::Colour(0xFF00FF41),     // sliderFill
        juce::Colour(0xFF008F11),         // sliderOutline
        juce::Colour(0xFFFFFFFF),         // sliderThumb
        juce::Colour(0xff31422d),         // sliderTrack
        juce::Colour(0xFF00FF41),         // labelText
        juce::Colour(0xFF008F11),         // labelShadow
        juce::Colour(0xff272727),         // bgCenter
        juce::Colour(0xFF000000),         // bgEdge
        juce::Colour(0xFF008F11),         // buttonColor
        juce::Colour(0xFF00FF41),         // buttonHoverColor
        juce::FontOptions("Lucidia Console", 10.0f, juce::Font::bold), // labelFont
        juce::Colours::black.withAlpha(0.95f), // setttingsOverlay
        juce::Colours::black.withAlpha(0.8f),  // presetOverlay
        juce::Colour(0xFF00FF41)};             // scrollbarThumb
}
} // namespace PluginTheme