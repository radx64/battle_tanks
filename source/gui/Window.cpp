#include "gui/Window.hpp"

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui
{

Window::Window()
: killed_{false}
, focused_{false}
, style_(BasicStyleSheetFactory::create())
{   
    shape_.setFillColor(style_.getInactiveWindowColor());
    shape_.setOutlineColor(style_.getOutlineColor());
    shape_.setOutlineThickness(style_.getOutlineThickness());
}

void Window::setSize(const sf::Vector2f& size)
{
    shape_.setSize(size);
    updateGlobalPosition();
}

sf::Vector2f Window::getSize()
{
    return shape_.getSize();
}

bool Window::isInside(const sf::Vector2f point)
{
    auto size = shape_.getSize();
    auto position = shape_.getPosition();

    return ((point.x >= position.x && point.x < position.x + size.x)
        && (point.y >= position.y && point.y < position.y + size.y));
}

void Window::onRender(sf::RenderWindow& renderWindow)
{
    shape_.setPosition(global_position_);
    renderWindow.draw(shape_);
}

float Window::getWidth()
{
    return shape_.getGlobalBounds().width;
}

float Window::getHeight()
{
    return shape_.getGlobalBounds().height;
}

bool Window::onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked)
{
    (void) mousePosition;
    (void) isLeftClicked;
    return false;
}

void Window::close()
{
    killed_ = true;
}

bool Window::isDead()
{
    return killed_;
}

void Window::focus()
{
    focused_ = true;
    shape_.setFillColor(style_.getWindowColor());
}

void Window::defocus()
{
    focused_ = false;
    shape_.setFillColor(style_.getInactiveWindowColor());
}

bool Window::hasFocus()
{
    return focused_;
}

}  // namespace gui