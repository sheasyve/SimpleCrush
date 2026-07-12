#pragma once
#include <JuceHeader.h>

namespace PluginTheme {

struct ThemeProps {
    juce::Colour sliderFill;
    juce::Colour sliderOutline;
    juce::Colour sliderThumb;
    juce::Colour sliderTrack;
    juce::Colour labelText;
    juce::Colour bgCenter;
    juce::Colour bgEdge;
    juce::Colour settings;
    juce::Colour settingsHover;
    juce::FontOptions labelFont;
};

inline ThemeProps getThemeProps(int themeId) {
    if (themeId == 1) { // Vaporwave
        return {juce::Colour(0xFFFF2A6D),
                juce::Colour(0xFF05D9E8),
                juce::Colour(0xFF01FFF4),
                juce::Colour(0xff14505f),
                juce::Colour(0xFF05D9E8),
                juce::Colour(0xFF390B8A),
                juce::Colour(0xFF000000),
                juce::Colour(0xFFCCCCCC).withAlpha(0.3f),
                juce::Colour(0xFFCCCCCC).withAlpha(0.5f),
                juce::FontOptions(15.0f, juce::Font::bold).withName("Trebuchet MS")};
    }

    if (themeId == 2) { // Panda Trueno
        return {juce::Colour(0xFFFFFFFF),
                juce::Colour(0xFF1A1A1A),
                juce::Colour(0xFFE50000),
                juce::Colour(0xFF333333),
                juce::Colour(0xFFFFFFFF),
                juce::Colour(0xFF2B2B2B),
                juce::Colour(0xFF050505),
                juce::Colour(0xFFFFFFFF).withAlpha(0.3f),
                juce::Colour(0xFFE50000),
                juce::FontOptions(15.0f, juce::Font::bold | juce::Font::italic).withName("Arial")};
    }

    if (themeId == 3) { // Studio Light
        return {juce::Colour(0xFF333333),
                juce::Colour(0xFFAAAAAA),
                juce::Colour(0xFF333333),
                juce::Colour(0xFFE0E0E0),
                juce::Colour(0xFF222222),
                juce::Colour(0xFFF9F9F9),
                juce::Colour(0xFFDCDCDC),
                juce::Colour(0xFF222222).withAlpha(0.3f),
                juce::Colour(0xFF333333),
                juce::FontOptions(15.0f, juce::Font::bold).withName("Segoe UI")};
    }

    if (themeId == 4) { // Studio Dark
        return {juce::Colour(0xffd6d6d6), 
                juce::Colour(0xFF4C4C4C), 
                juce::Colour(0xFFD4D4D4), 
                juce::Colour(0xFF2B2B2B), 
                juce::Colour(0xFFD4D4D4), 
                juce::Colour(0xFF202020), 
                juce::Colour(0xFF202020), 
                juce::Colour(0xFFD4D4D4).withAlpha(0.3f), 
                juce::Colour(0xffd6d6d6), 
                juce::FontOptions(15.0f, juce::Font::plain).withName("Segoe UI")};
    }

    if (themeId == 5) { // Arctic Freeze
        return {juce::Colour(0xFF74B3CE),
                juce::Colour(0xFF508991),
                juce::Colour(0xFFFFFFFF),
                juce::Colour(0xFF172A3A),
                juce::Colour(0xFF98C1D9),
                juce::Colour(0xFF172A3A),
                juce::Colour(0xFF09141C),
                juce::Colour(0xFF98C1D9).withAlpha(0.3f),
                juce::Colour(0xFFFFFFFF),
                juce::FontOptions(15.0f, juce::Font::bold).withName("Trebuchet MS")};
    }

    if (themeId == 6) { // Vintage Analog
        return {juce::Colour(0xFFE6A822),
                juce::Colour(0xFF2A2E2A),
                juce::Colour(0xFFD9D9D9),
                juce::Colour(0xFF3A3F3A),
                juce::Colour(0xFFE6A822),
                juce::Colour(0xFF4A4F4A),
                juce::Colour(0xFF1D1F1D),
                juce::Colour(0xFFE6A822).withAlpha(0.3f),
                juce::Colour(0xFFE6A822),
                juce::FontOptions(15.0f, juce::Font::bold).withName("Tahoma")};
    }

    if (themeId == 7) { // Retro Caramel
        return {
            juce::Colour(0xFFAD7640),
            juce::Colour(0xFF6B4226),
            juce::Colour(0xFFC99966),
            juce::Colour(0xFF5C3822),
            juce::Colour(0xFFE3D3B5),
            juce::Colour(0xFF2E1A0F),
            juce::Colour(0xFF120804),
            juce::Colour(0xFFCCCCCC).withAlpha(0.3f),
            juce::Colour(0xFFCCCCCC).withAlpha(0.5f),
            juce::FontOptions(15.0f, juce::Font::italic | juce::Font::bold).withName("Helvetica")};
    }

    // Midnight Hacker
    return {juce::Colour(0xFF00FF41),
            juce::Colour(0xFF008F11),
            juce::Colour(0xFFFFFFFF),
            juce::Colour(0xff31422d),
            juce::Colour(0xFF00FF41),
            juce::Colour(0xff272727),
            juce::Colour(0xFF000000),
            juce::Colour(0xFF00FF41).withAlpha(0.3f),
            juce::Colour(0xFF00FF41),
            juce::FontOptions(15.0f, juce::Font::bold).withName("Lucida Console")};
}
} // namespace PluginTheme