#ifndef GUI_LABEL_HPP_
#define GUI_LABEL_HPP_

#include <SFML/Graphics.hpp>

#include "gui/Element.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{

class Label : public Element
{
public:
    Label(const std::string_view& text, Element* parent)
    : Element(parent)
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

    void on_render(sf::RenderWindow& renderWindow) override
    {
        text_.setPosition(global_postition_);
        renderWindow.draw(text_);
    }

    float get_width() override
    {
        return text_.getGlobalBounds().width;
    }

    void set_text(const sf::String& text)
    {
        text_.setString(text);
    }

protected:
    sf::Text text_;
};

}  // namespace gui

#endif  // GUI_LABEL_HPP_