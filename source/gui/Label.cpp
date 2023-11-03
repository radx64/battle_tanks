#include "gui/Label.hpp"

#include "gui/StyleSheet.hpp"

namespace gui 
{

Label::Label(const std::string_view& text)
{
    auto style = BasicStyleSheetFactory::create();
    text_.setFont(style.getFont());
    text_.setCharacterSize(style.getFontSize());
    text_.setFillColor(style.getFontColor());
    text_.setOutlineColor(style.getOutlineColor());
    text_.setOutlineThickness(style.getOutlineThickness());
    text_.setString(text.data());       
}

void Label::onRender(sf::RenderWindow& renderWindow)
{
    text_.setPosition(global_position_);
    renderWindow.draw(text_);
}

float Label::getWidth()
{
    return text_.getGlobalBounds().width;
}

float Label::getHeight()
{
    return text_.getGlobalBounds().height;
}

void Label::setText(const sf::String& text)
{
    text_.setString(text);
}

bool Label::onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked)
{
    (void) mousePosition;
    (void) isLeftClicked;
    return false;
}

}  // namespace gui