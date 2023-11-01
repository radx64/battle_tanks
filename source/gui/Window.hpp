#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{

class Window : public Component
{
public:
    Window(Component* parent)
    : Component(parent)
    {
        auto style = BasicStyleSheetFactory::create();    
        shape_.setFillColor(style.getWindowColor());
        shape_.setOutlineColor(style.getOutlineColor());
        shape_.setOutlineThickness(style.getOutlineThickness());
    }

    Window() : Window(nullptr)
    {}

    void setSize(const sf::Vector2f& size)
    {
        shape_.setSize(size);
        updateGlobalPosition();
    }

    void onRender(sf::RenderWindow& renderWindow) override
    {
        shape_.setPosition(global_position_);
        renderWindow.draw(shape_);
    }

    float getWidth() override
    {
        return shape_.getGlobalBounds().width;
    }

    bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked)
    {
        (void) mousePosition;
        (void) isLeftClicked;
        return false;
    }

protected:
    sf::RectangleShape shape_;
};

}  // namespace gui

#endif  // GUI_WINDOW_HPP_