#include "gui/StyleSheet.hpp"

#include <SFML/Graphics/Color.hpp>

#include "gui/FontLibrary.hpp"

namespace gui
{
StyleSheet::StyleSheet() 
: font_(gui::FontLibrary::get("default"))
, font_size_(12)
, font_color_(sf::Color::Black)
, outline_color_(sf::Color::Black)
, outline_thickness_(0)
, window_color_(sf::Color(127, 127, 127, 200))
, window_inactive_color_(sf::Color(100, 100, 100, 200))
, window_top_bar_color_(sf::Color(100, 100, 100, 255))
, window_inanctive_top_bar_color_(sf::Color(100, 100, 100, 255))
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

const sf::Color& StyleSheet::getWindowHeaderColor() const
{
    return window_top_bar_color_;
}

void StyleSheet::setTopBarWindowColor(const sf::Color& color)
{
    window_top_bar_color_ = color;
}

const sf::Color& StyleSheet::getInactiveWindowHeaderColor() const
{
    return window_inanctive_top_bar_color_;
}

void StyleSheet::setInactiveTopBarWindowColor(const sf::Color& color)
{
    window_inanctive_top_bar_color_ = color;
}

const StyleSheet& BasicStyleSheetFactory::instance()
{
    if (stylesheet_) return *stylesheet_;
    else
    {
        stylesheet_ = new StyleSheet();
        stylesheet_->setFont(gui::FontLibrary::get("armata"));
        stylesheet_->setFontSize(16);
        stylesheet_->setFontColor(sf::Color::Black);
        stylesheet_->setOutlineColor(sf::Color(127,127,127,255));
        stylesheet_->setOutlineThickness(1);
        stylesheet_->setWindowColor(sf::Color(200,200,200,200));
        stylesheet_->setInactiveWindowColor(sf::Color(100,100,100,200));
        stylesheet_->setTopBarWindowColor(sf::Color(230,100,100,255));
        stylesheet_->setInactiveTopBarWindowColor(sf::Color(130,100,100,255));
        return *stylesheet_;
    }
}

StyleSheet* BasicStyleSheetFactory::stylesheet_= nullptr;

}  // namespace gui
