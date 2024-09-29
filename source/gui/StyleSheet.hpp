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
    uint32_t fontSize_;
    sf::Color fontColor_;
    sf::Color outlineColor_;
    uint32_t outlineThickness_;

    sf::Color windowColor_;
    sf::Color windowInactiveColor_;
    sf::Color windowTopBarColor_;
    sf::Color windowInanctiveTopBarColor_;
};

class BasicStyleSheetFactory
{
public:
    static const StyleSheet& instance();

private:
    static StyleSheet* stylesheet_;
};

}  // namespace gui
