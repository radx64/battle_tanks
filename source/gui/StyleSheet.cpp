#include "gui/StyleSheet.hpp"

#include <SFML/Graphics/Color.hpp>

#include "gui/FontLibrary.hpp"

namespace gui
{
StyleSheet::StyleSheet() 
: font_(gui::FontLibrary::get("default"))
, fontSize_(12)
, fontColor_(sf::Color::Black)
, outlineColor_(sf::Color::Black)
, outlineThickness_(0)
, windowColor_(sf::Color(127, 127, 127, 200))
, windowInactiveColor_(sf::Color(100, 100, 100, 200))
, windowHeaderColor_(sf::Color(100, 100, 100, 255))
, windowInanctiveHeaderColor_(sf::Color(100, 100, 100, 255))
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
    return fontSize_;
}

void StyleSheet::setFontSize(const uint32_t size)
{
    fontSize_ = size;
}

const sf::Color& StyleSheet::getFontColor() const
{
    return fontColor_;
}

void StyleSheet::setFontColor(const sf::Color& color)
{
    fontColor_ = color;
}

const sf::Color& StyleSheet::getOutlineColor() const
{
    return outlineColor_;
}

void StyleSheet::setOutlineColor(const sf::Color& color)
{
    outlineColor_ = color;
}

uint32_t StyleSheet::getOutlineThickness() const
{
    return outlineThickness_;
}

void StyleSheet::setOutlineThickness(const uint32_t thickness)
{
    outlineThickness_ = thickness;
}

const sf::Color& StyleSheet::getWindowColor() const
{
    return windowColor_;
}

void StyleSheet::setWindowColor(const sf::Color& color)
{
    windowColor_ = color;
}  

const sf::Color& StyleSheet::getInactiveWindowColor() const
{
    return windowInactiveColor_;
}

void StyleSheet::setInactiveWindowColor(const sf::Color& color)
{
    windowInactiveColor_ = color;
}  

const sf::Color& StyleSheet::getWindowHeaderColor() const
{
    return windowHeaderColor_;
}

void StyleSheet::setWindowHeaderColor(const sf::Color& color)
{
    windowHeaderColor_ = color;
}

const sf::Color& StyleSheet::getInactiveWindowHeaderColor() const
{
    return windowInanctiveHeaderColor_;
}

void StyleSheet::setInactiveWindowHeaderColor(const sf::Color& color)
{
    windowInanctiveHeaderColor_ = color;
}

const StyleSheet& BasicStyleSheetFactory::instance()
{
    if (stylesheet_) return *stylesheet_;
    else
    {
        stylesheet_ = std::make_unique<StyleSheet>();
        stylesheet_->setFont(gui::FontLibrary::get("ibmVGA"));
        stylesheet_->setFontSize(22);
        stylesheet_->setFontColor(sf::Color::Black);
        stylesheet_->setOutlineColor(sf::Color(127,127,127,255));
        stylesheet_->setOutlineThickness(1);
        stylesheet_->setWindowColor(sf::Color(200,200,200,200));
        stylesheet_->setInactiveWindowColor(sf::Color(100,100,100,200));
        stylesheet_->setWindowHeaderColor(sf::Color(0,0,128,255));
        stylesheet_->setInactiveWindowHeaderColor(sf::Color(140,140,140,255));
        return *stylesheet_;
    }
}

void BasicStyleSheetFactory::destroy()
{
    stylesheet_.reset();
}

std::unique_ptr<StyleSheet> BasicStyleSheetFactory::stylesheet_= nullptr;

}  // namespace gui
