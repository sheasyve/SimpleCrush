#include "plugin_theme.h"

namespace PluginTheme {

ThemeManager::ThemeManager() {
    loadDefaultFallbackTheme();
    refreshThemes();
}

juce::Colour ThemeManager::parseHexColour(const juce::String &hexString) {
    // Parse the color so that I can use rgba format in the json file. The format is #RRGGBBAA or #RRGGBB
    juce::String localHex = hexString;
    if (localHex.startsWithChar('#')) {
        localHex = localHex.substring(1);
        if (localHex.length() == 8) {
            juce::String alpha = localHex.substring(6, 8);
            juce::String rgb = localHex.substring(0, 6);
            localHex = alpha + rgb;
        } else if (localHex.length() == 6) {
            localHex = "FF" + localHex;
        }
        localHex = "0x" + localHex;
    }
    return juce::Colour::fromString(localHex);
}

ThemeProps ThemeManager::loadThemeFromFile(const juce::File &file) {
    ThemeProps theme;
    if (!file.existsAsFile()) return theme;

    var parsedJson = juce::JSON::parse(file);
    if (auto *obj = parsedJson.getDynamicObject()) {
        theme.name = obj->getProperty("name").toString();
        theme.id = static_cast<int>(obj->getProperty("id"));

        auto colorsVar = obj->getProperty("colors");
        if (auto *colors = colorsVar.getDynamicObject()) {
            theme.sliderFill = parseHexColour(colors->getProperty("sliderFill").toString());
            theme.sliderOutline = parseHexColour(colors->getProperty("sliderOutline").toString());
            theme.sliderThumb = parseHexColour(colors->getProperty("sliderThumb").toString());
            theme.sliderTrack = parseHexColour(colors->getProperty("sliderTrack").toString());
            theme.labelText = parseHexColour(colors->getProperty("labelText").toString());
            theme.labelShadow = parseHexColour(colors->getProperty("labelShadow").toString());
            theme.bgCenter = parseHexColour(colors->getProperty("bgCenter").toString());
            theme.bgEdge = parseHexColour(colors->getProperty("bgEdge").toString());
            theme.buttonColor = parseHexColour(colors->getProperty("buttonColor").toString());
            theme.buttonHoverColor = parseHexColour(colors->getProperty("buttonHoverColor").toString());
            theme.settingsOverlay = parseHexColour(colors->getProperty("settingsOverlay").toString());
            theme.presetOverlay = parseHexColour(colors->getProperty("presetOverlay").toString());
            theme.scrollbarThumb = parseHexColour(colors->getProperty("scrollbarThumb").toString());
        }

        auto fontVar = obj->getProperty("font");
        if (auto *font = fontVar.getDynamicObject()) {
            juce::String fontName = font->getProperty("name").toString();
            float height = static_cast<float>(font->getProperty("height"));
            bool isBold = static_cast<bool>(font->getProperty("isBold"));
            bool isItalic = static_cast<bool>(font->getProperty("isItalic"));

            int styleFlags = juce::Font::plain;
            if (isBold) styleFlags |= juce::Font::bold;
            if (isItalic) styleFlags |= juce::Font::italic;

            theme.labelFont = juce::FontOptions(height, styleFlags).withName(fontName);
        }

        auto logoFontVar = obj->getProperty("logoFont");
        if (auto *logoFont = logoFontVar.getDynamicObject()) {
            juce::String fontName = logoFont->getProperty("name").toString();
            bool isBold = static_cast<bool>(logoFont->getProperty("isBold"));
            bool isItalic = static_cast<bool>(logoFont->getProperty("isItalic"));

            int styleFlags = juce::Font::plain;
            if (isBold) styleFlags |= juce::Font::bold;
            if (isItalic) styleFlags |= juce::Font::italic;

            theme.logoFont = juce::FontOptions(20.0f, styleFlags).withName(fontName);
        } else {
            theme.logoFont = juce::FontOptions(20.0f, juce::Font::bold).withName("Arial");
        }
    }
    return theme;
}

void ThemeManager::refreshThemes() {
    cachedThemes.clear();
    auto files = getThemesDirectory().findChildFiles(juce::File::findFiles, false, "*.json");

    for (const auto &file : files) {
        auto props = loadThemeFromFile(file);
        if (props.name.isNotEmpty()) { cachedThemes[props.id] = props; }
    }
}

ThemeProps ThemeManager::getThemeProps(int themeId) const {
    auto it = cachedThemes.find(themeId);
    if (it != cachedThemes.end()) return it->second;

    return defaultTheme;
}

std::vector<std::pair<juce::String, int>> ThemeManager::getAvailableThemes() const {
    std::vector<std::pair<juce::String, int>> list;
    for (const auto &[id, props] : cachedThemes) { list.push_back({props.name, id}); }
    return list;
}

void ThemeManager::loadDefaultFallbackTheme() {
    // Hardcoded default theme values
    defaultTheme.name = "Panda Trueno (Default)";
    defaultTheme.id = 1;
    defaultTheme.sliderFill = juce::Colour(0xFFFFFFFF);
    defaultTheme.sliderOutline = juce::Colour(0xFF1A1A1A);
    defaultTheme.sliderThumb = juce::Colour(0xFFE50000);
    defaultTheme.sliderTrack = juce::Colour(0xFF333333);
    defaultTheme.labelText = juce::Colour(0xFFFFFFFF);
    defaultTheme.labelShadow = juce::Colour(0x7E2B2B2B);
    defaultTheme.bgCenter = juce::Colour(0xFF2B2B2B);
    defaultTheme.bgEdge = juce::Colour(0xFF050505);
    defaultTheme.buttonColor = juce::Colour(0xBDFFFFFF);
    defaultTheme.buttonHoverColor = juce::Colour(0xFFE50000);
    defaultTheme.labelFont = juce::FontOptions(15.0f, juce::Font::bold | juce::Font::italic).withName("Arial");
    defaultTheme.logoFont = juce::FontOptions(20.0f, juce::Font::bold).withName("Arial");
    defaultTheme.settingsOverlay = juce::Colours::black.withAlpha(0.90f);
    defaultTheme.presetOverlay = juce::Colours::black.withAlpha(0.80f);
    defaultTheme.scrollbarThumb = juce::Colour(0xFFE50000);
}

void ThemeManager::loadSettings(const juce::File &settingsFile) {
    if (settingsFile.existsAsFile()) {
        if (std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(settingsFile)) {
            juce::String savedPath = xml->getStringAttribute("DataFolder");
            if (savedPath.isNotEmpty()) {
                juce::File savedDir(savedPath);
                if (savedDir.exists() && savedDir.isDirectory()) {
                    customThemesDir = savedDir.getChildFile("Themes");
                    if (!customThemesDir.exists()) customThemesDir.createDirectory();
                }
            }
        }
    }
    refreshThemes();
}

juce::File ThemeManager::getThemesDirectory() {
    if (customThemesDir.exists()) return customThemesDir;
    auto dir =
        juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("SimpleCrush/Themes");
    if (!dir.exists()) dir.createDirectory();
    return dir;
}

} // namespace PluginTheme