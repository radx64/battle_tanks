#include "gui/StyleSheet.hpp"

#include <SFML/Graphics/Color.hpp>

#include "gui/FontLibrary.hpp"

namespace gui
{
StyleSheet::StyleSheet() 
: font_(gui::FontLibrary::get("default"))
, font_size_(10)
, font_color_(sf::Color::Black)
, outline_color_(sf::Color::Black)
, outline_thickness_(0)
, window_color_(sf::Color(127, 127, 127, 200))
, window_inactive_color_(sf::Color(100, 100, 100, 200))
{ }

sf::Font& StyleSheet::getFont() const
{
    return font_;
}

void StyleSheet::setFont(const sf::Font& font)
{
    font_ = font;
}

uint32_t StyleSheet::getFontSize() const
{
    return font_size_;
}

void StyleSheet::setFontSize(const uint32_t size)
{
    font_size_ = size;
}

const sf::Color& StyleSheet::getFontColor() const
{
    return font_color_;
}

void StyleSheet::setFontColor(const sf::Color& color)
{
    font_color_ = color;
}

const sf::Color& StyleSheet::getOutlineColor() const
{
    return outline_color_;
}

void StyleSheet::setOutlineColor(const sf::Color& color)
{
    outline_color_ = color;
}

uint32_t StyleSheet::getOutlineThickness() const
{
    return outline_thickness_;
}

void StyleSheet::setOutlineThickness(const uint32_t thickness)
{
    outline_thickness_ = thickness;
}

const sf::Color& StyleSheet::getWindowColor() const
{
    return window_color_;
}

void StyleSheet::setWindowColor(const sf::Color& color)
{
    window_color_ = color;
}  

const sf::Color& StyleSheet::getInactiveWindowColor() const
{
    return window_inactive_color_;
}

void StyleSheet::setInactiveWindowColor(const sf::Color& color)
{
    window_inactive_color_ = color;
}  

const StyleSheet BasicStyleSheetFactory::create()
{
    StyleSheet style;
    style.setFont(gui::FontLibrary::get("armata"));
    style.setFontSize(14);
    style.setFontColor(sf::Color::Black);
    style.setOutlineColor(sf::Color(127,127,127,255));
    style.setOutlineThickness(1);
    style.setWindowColor(sf::Color(200,200,200,200));
    style.setInactiveWindowColor(sf::Color(100,100,100,200));
    return style;
}

}  // namespace gui
