#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

#include <SFML/Graphics.hpp>

#include "gui/Element.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{

class Window : public Element
{
public:
    Window(Element* parent)
    : Element(parent)
    {
        auto style = BasicStyleSheetFactory::create();    
        rectangle_.setFillColor(style.getWindowColor());
        rectangle_.setOutlineColor(style.getOutlineColor());
        rectangle_.setOutlineThickness(style.getOutlineThickness());
    }

    Window() : Window(nullptr)
    {}

    void set_size(const sf::Vector2f& size)
    {
        rectangle_.setSize(size);
        update_global_position();
    }

    void on_render(sf::RenderWindow& renderWindow) override
    {
        rectangle_.setPosition(global_postition_);
        renderWindow.draw(rectangle_);
    }

    float get_width() override
    {
        return rectangle_.getGlobalBounds().width;
    }

protected:
    sf::RectangleShape rectangle_;
};

}  // namespace gui

#endif  // GUI_WINDOW_HPP_