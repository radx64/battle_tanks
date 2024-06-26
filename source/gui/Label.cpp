#include "gui/Label.hpp"

#include "gui/StyleSheet.hpp"

namespace gui 
{

Label::Label(const std::string_view& text)
{
    auto style = BasicStyleSheetFactory::instance();
    text_.setFont(style.getFont());
    text_.setCharacterSize(style.getFontSize());
    text_.setFillColor(style.getFontColor());
    text_.setOutlineColor(style.getOutlineColor());
    text_.setOutlineThickness(style.getOutlineThickness());
    setText(text.data()); 
}

void Label::onRender(sf::RenderTexture& renderWindow)
{
    text_.setPosition(Component::getGlobalPosition()); // TODO: Position recalculation of text should not be needed
    renderWindow.draw(text_);
}

void Label::setText(const sf::String& text)
{
    text_.setString(text);
    bounds_.width = text_.getGlobalBounds().width;
    bounds_.height = text_.getCharacterSize();
    Component::updateGlobalPosition();
}

}  // namespace gui