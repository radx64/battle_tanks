#pragma once

#include <cstdint>
#include <SFML/Graphics/Color.hpp>

#include "gui/FontLibrary.hpp"

namespace gui
{

// This is a crude stylesheet for GUI styling
// I'll replace it with something more sophisticated later
class StyleSheet
{
public:
    StyleSheet();

    sf::Font& getFont() const;
    void setFont(const sf::Font& font);
    uint32_t getFontSize() const;
    void setFontSize(const uint32_t size);
    const sf::Color& getFontColor() const;
    void setFontColor(const sf::Color& color);
    const sf::Color& getOutlineColor() const;
    void setOutlineColor(const sf::Color& color);
    uint32_t getOutlineThickness() const;
    void setOutlineThickness(const uint32_t thickness);
    const sf::Color& getWindowColor() const;
    void setWindowColor(const sf::Color& color);
    const sf::Color& getInactiveWindowColor() const;
    void setInactiveWindowColor(const sf::Color& color);
    const sf::Color& getWindowHeaderColor() const;
    void setTopBarWindowColor(const sf::Color& color);
    const sf::Color& getInactiveWindowHeaderColor() const;
    void setInactiveTopBarWindowColor(const sf::Color& color);
protected:
    sf::Font& font_;
    uint32_t font_size_;
    sf::Color font_color_;
    sf::Color outline_color_;
    uint32_t outline_thickness_;

    sf::Color window_color_;
    sf::Color window_inactive_color_;
    sf::Color window_top_bar_color_;
    sf::Color window_inanctive_top_bar_color_;
};

class BasicStyleSheetFactory
{
public:
    static const StyleSheet& instance();

private:
    static StyleSheet* stylesheet_;
};

}  // namespace gui

