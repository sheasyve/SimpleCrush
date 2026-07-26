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

    void paint(juce::Graphics& g) override {
        g.fillAll(currentOverlayColor); 
    }
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
            juce::Colour(0xFFCCCCCC).withAlpha(0.5f),
            juce::Colour(0xFFCCCCCC).withAlpha(0.7f),
            juce::FontOptions(15.0f, juce::Font::bold | juce::Font::italic).withName("Arial"),
            juce::Colours::black.withAlpha(0.9f),
            juce::Colours::black.withAlpha(0.8f),        
            juce::Colour(0xFF01FFF4)};                
    }

    if (themeId == 2) { // Studio Dark
        return {juce::Colour(0xffd6d6d6),
            juce::Colour(0xFF4C4C4C),
            juce::Colour(0xFFD4D4D4),
            juce::Colour(0xff575757),
            juce::Colour(0xFFD4D4D4),
            juce::Colour(0xFF202020),
            juce::Colour(0xFF202020),
            juce::Colour(0xFFD4D4D4).withAlpha(0.5f),
            juce::Colour(0xfff0f0f0),
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
            juce::Colour(0xFFDCDCDC),
            juce::Colour(0xFF222222).withAlpha(0.5f),
            juce::Colour(0xff0e0e0e),
            juce::FontOptions(15.0f, juce::Font::bold).withName("Arial"),
            juce::Colour(0xFFE0E0E0).withAlpha(0.93f),
            juce::Colour(0xFFE0E0E0).withAlpha(0.8f), 
            juce::Colour(0xFF333333)};                 
    }
    if (themeId == 4) { // Panda Trueno
        return {juce::Colour(0xFFFFFFFF),
            juce::Colour(0xFF1A1A1A),
            juce::Colour(0xFFE50000),
            juce::Colour(0xFF333333),
            juce::Colour(0xFFFFFFFF),
            juce::Colour(0xFF2B2B2B),
            juce::Colour(0xFF050505),
            juce::Colour(0xFFFFFFFF).withAlpha(0.5f),
            juce::Colour(0xFFE50000),
            juce::FontOptions(15.0f, juce::Font::bold | juce::Font::italic).withName("Arial"),
            juce::Colours::black.withAlpha(0.90f),  
            juce::Colours::black.withAlpha(0.8f),     
            juce::Colour(0xFFE50000)};                 
    }
    if (themeId == 5) { // Retro Caramel
        return {juce::Colour(0xFFAD7640),
            juce::Colour(0xFF6B4226),
            juce::Colour(0xFFC99966),
            juce::Colour(0xFF5C3822),
            juce::Colour(0xFFE3D3B5),
            juce::Colour(0xFF2E1A0F),
            juce::Colour(0xFF120804),
            juce::Colour(0xFFCCCCCC).withAlpha(0.5f),
            juce::Colour(0xFFCCCCCC).withAlpha(0.7f),
            juce::FontOptions(15.0f, juce::Font::italic | juce::Font::bold).withName("Helvetica"),
            juce::Colour(0xFF120804).withAlpha(0.9f), 
            juce::Colour(0xFF120804).withAlpha(0.8f), 
            juce::Colour(0xFFC99966)};                 
    }
    if (themeId == 6) { // Arctic Freeze
        return {juce::Colour(0xFF74B3CE),
            juce::Colour(0xFF508991),
            juce::Colour(0xFFFFFFFF),
            juce::Colour(0xFF172A3A),
            juce::Colour(0xFF98C1D9),
            juce::Colour(0xFF172A3A),
            juce::Colour(0xFF09141C),
            juce::Colour(0xFF98C1D9).withAlpha(0.5f),
            juce::Colour(0xFF98C1D9).withAlpha(0.7f),
            juce::FontOptions(15.0f, juce::Font::bold).withName("Trebuchet MS"),
            juce::Colour(0xFF09141C).withAlpha(0.95f), 
            juce::Colour(0xFF09141C).withAlpha(0.8f), 
            juce::Colour(0xFFFFFFFF)};                 
    }

    // Midnight Hacker
    return {juce::Colour(0xFF00FF41),
        juce::Colour(0xFF008F11),
        juce::Colour(0xFFFFFFFF),
        juce::Colour(0xff31422d),
        juce::Colour(0xFF00FF41),
        juce::Colour(0xff272727),
        juce::Colour(0xFF000000),
        juce::Colour(0xFF00FF41).withAlpha(0.5f),
        juce::Colour(0xFF00FF41).withAlpha(0.7f),
        juce::FontOptions(12.0f, juce::Font::plain).withName("Consolas"),
        juce::Colours::black.withAlpha(0.95f),
        juce::Colours::black.withAlpha(0.8f),              
        juce::Colour(0xFF00FF41)};                    
}
} // namespace PluginTheme