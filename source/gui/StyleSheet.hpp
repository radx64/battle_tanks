#ifndef GUI_STYLESHEET_HPP_
#define GUI_STYLESHEET_HPP_

#include <cstdint>
#include <SFML/Graphics/Color.hpp>

#include "gui/FontLibrary.hpp"

namespace gui
{

class StyleSheet
{
public:
    StyleSheet() 
    : font_(gui::FontLibrary::get("default"))
    , font_size_(20)
    , font_color_(sf::Color::Black)
    , outline_color_(sf::Color::Black)
    , outline_thickness_(0)
    , window_color_(sf::Color(127, 127, 127, 200))
    { }

    sf::Font& getFont() const
    {
        return font_;
    }

    void setFont(const sf::Font& font)
    {
        font_ = font;
    }

    uint32_t getFontSize() const
    {
        return font_size_;
    }

    void setFontSize(const uint32_t size)
    {
        font_size_ = size;
    }

    const sf::Color& getFontColor() const
    {
        return font_color_;
    }

    void setFontColor(const sf::Color& color)
    {
        font_color_ = color;
    }

    const sf::Color& getOutlineColor() const
    {
        return outline_color_;
    }

    void setOutlineColor(const sf::Color& color)
    {
        outline_color_ = color;
    }

    uint32_t getOutlineThickness() const
    {
        return outline_thickness_;
    }

    void setOutlineThickness(const uint32_t thickness)
    {
        outline_thickness_ = thickness;
    }

    const sf::Color& getWindowColor() const
    {
        return window_color_;
    }

    void setWindowColor(const sf::Color& color)
    {
        window_color_ = color;
    }  


protected:
    sf::Font& font_;
    uint32_t font_size_;
    sf::Color font_color_;
    sf::Color outline_color_;
    uint32_t outline_thickness_;

    sf::Color window_color_;
};

class BasicStyleSheetFactory
{
public:
    static const StyleSheet create()
    {
        StyleSheet style;
        style.setFont(gui::FontLibrary::get("armata"));
        style.setFontSize(20);
        style.setFontColor(sf::Color::Black);
        style.setOutlineColor(sf::Color(127,127,127,255));
        style.setOutlineThickness(1);
        style.setWindowColor(sf::Color(200,200,200,200));
        return style;
    }
};

}  // namespace gui

#endif  // GUI_STYLESHEET_HPP_