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
        return {juce::Colour(0xFFFFFFFF),
            juce::Colour(0xFF1A1A1A),
            juce::Colour(0xFFE50000),
            juce::Colour(0xFF333333),
            juce::Colour(0xFFFFFFFF),
            juce::Colour(0xFF2B2B2B),
            juce::Colour(0xFF2B2B2B),
            juce::Colour(0xFF050505),
            juce::Colour(0xbdffffff),
            juce::Colour(0xFFE50000),
            juce::FontOptions(15.0f, juce::Font::bold | juce::Font::italic).withName("Arial"),
            juce::Colours::black.withAlpha(0.90f),
            juce::Colours::black.withAlpha(0.8f),
            juce::Colour(0xFFE50000)};
    }

    if (themeId == 2) { // Studio Dark
        return {juce::Colour(0xffd6d6d6),
            juce::Colour(0xFF4C4C4C),
            juce::Colour(0xFFD4D4D4),
            juce::Colour(0xff575757),
            juce::Colour(0xfff1f1f1),
            juce::Colour(0xFFF9F9F9),
            juce::Colour(0xFF202020),
            juce::Colour(0xff131313),
            juce::Colour(0xdea1a1a1),
            juce::Colour(0xffebebeb),
            juce::FontOptions(15.0f, juce::Font::bold).withName("Arial"),
            juce::Colours::black.withAlpha(0.9f),
            juce::Colours::black.withAlpha(0.8f),
            juce::Colour(0xFFD4D4D4)};
    }

    if (themeId == 3) { // Studio Light
        return {juce::Colour(0xFF333333),
            juce::Colour(0xFFAAAAAA),
            juce::Colour(0xFF333333),
            juce::Colour(0xffbbbbbb),
            juce::Colour(0xFF222222),
            juce::Colour(0xFFF9F9F9),
            juce::Colour(0xFFF9F9F9),
            juce::Colour(0xFFDCDCDC),
            juce::Colour(0xff080808),
            juce::Colour(0xff8d8d8d),
            juce::FontOptions(15.0f, juce::Font::bold).withName("Arial"),
            juce::Colour(0xFFE0E0E0).withAlpha(0.83f),
            juce::Colour(0xFFE0E0E0).withAlpha(0.83f),
            juce::Colour(0xFF333333)};
    }

    if (themeId == 4) { // Vaporwave
        return {juce::Colour(0xFFFF2A6D),
            juce::Colour(0xFF05D9E8),
            juce::Colour(0xFF01FFF4),
            juce::Colour(0xff14505f),
            juce::Colour(0xFF05D9E8),
            juce::Colour(0xFFFF2A6D),
            juce::Colour(0xFF390B8A),
            juce::Colour(0xFF000000),
            juce::Colour(0xFF05D9E8),
            juce::Colour(0xFFFF2A6D),
            juce::FontOptions(15.0f, juce::Font::bold | juce::Font::italic).withName("Arial"),
            juce::Colours::black.withAlpha(0.8f),
            juce::Colours::black.withAlpha(0.78f),
            juce::Colour(0xFF01FFF4)};
    }

    if (themeId == 5) { // Retro Caramel
        return {juce::Colour(0xFFC58B4E),
            juce::Colour(0xFF9A623B),
            juce::Colour(0xFFDFB382),
            juce::Colour(0xFF7B4A2E),
            juce::Colour(0xFFF0E3CB),
            juce::Colour(0xFFDFB382),
            juce::Colour(0xff644630),
            juce::Colour(0xFF2B1910),
            juce::Colour(0xFFDFB382),
            juce::Colour(0xFF8C6C4B),
            juce::FontOptions(15.0f, juce::Font::italic | juce::Font::bold).withName("Helvetica"),
            juce::Colour(0xFF2B1910).withAlpha(0.9f),
            juce::Colour(0xFF2B1910).withAlpha(0.8f),
            juce::Colour(0xFFDFB382)};
    }

    if (themeId == 6) { // Arctic Freeze
        return {juce::Colour(0xFF30C5FF),
            juce::Colour(0xFF0096C7),
            juce::Colour(0xFFFFFFFF),
            juce::Colour(0xFF0A2440),
            juce::Colour(0xFFADE8F4),
            juce::Colour(0xFF0A2440),
            juce::Colour(0xFF0A2440),
            juce::Colour(0xFF031020),
            juce::Colour(0xFF0096C7),
            juce::Colour(0xFF30C5FF),
            juce::FontOptions(16.0f, juce::Font::bold).withName("Trebuchet MS"),
            juce::Colour(0xFF031020).withAlpha(0.95f),
            juce::Colour(0xFF031020).withAlpha(0.8f),
            juce::Colour(0xFFFFFFFF)};
    }

    // Midnight Hacker
    return {juce::Colour(0xFF00FF41),
        juce::Colour(0xFF008F11),
        juce::Colour(0xFFFFFFFF),
        juce::Colour(0xff31422d),
        juce::Colour(0xFF00FF41),
        juce::Colour(0xFF008F11),
        juce::Colour(0xff272727),
        juce::Colour(0xFF000000),
        juce::Colour(0xFF008F11),
        juce::Colour(0xFF00FF41),
        juce::FontOptions("Lucidia Console", 10.0f, juce::Font::bold),
        juce::Colours::black.withAlpha(0.95f),
        juce::Colours::black.withAlpha(0.8f),
        juce::Colour(0xFF00FF41)};
}
} // namespace PluginTheme