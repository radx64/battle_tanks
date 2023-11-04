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
    setText(text.data()); 
}

void Label::onRender(sf::RenderWindow& renderWindow)
{
    text_.setPosition(Component::getGlobalPosition());
    renderWindow.draw(text_);
}

void Label::setText(const sf::String& text)
{
    text_.setString(text);
    bounds_.width = text_.getGlobalBounds().width;
    bounds_.height = text_.getGlobalBounds().height;
    Component::updateGlobalPosition();
}

bool Label::onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked)
{
    (void) mousePosition;
    (void) isLeftClicked;
    return false;
}

}  // namespace gui