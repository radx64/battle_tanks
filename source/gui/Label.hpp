#ifndef GUI_LABEL_HPP_
#define GUI_LABEL_HPP_

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{

class Label : public Component
{
public:
    Label(const std::string_view& text, Component* parent)
    : Component(parent)
    {
        auto style = BasicStyleSheetFactory::create();
        text_.setFont(style.getFont());
        text_.setCharacterSize(style.getFontSize());
        text_.setFillColor(style.getFontColor());
        text_.setOutlineColor(style.getOutlineColor());
        text_.setOutlineThickness(style.getOutlineThickness());
        text_.setString(text.data());       
    }

    Label(const std::string_view& text) : Label(text, nullptr)
    { }

    void onRender(sf::RenderWindow& renderWindow) override
    {
        text_.setPosition(global_position_);
        renderWindow.draw(text_);
    }

    float getWidth() override
    {
        return text_.getGlobalBounds().width;
    }

    void setText(const sf::String& text)
    {
        text_.setString(text);
    }

    bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked)
    {
        (void) mousePosition;
        (void) isLeftClicked;
        return false;
    }

protected:
    sf::Text text_;
};

}  // namespace gui

#endif  // GUI_LABEL_HPP_