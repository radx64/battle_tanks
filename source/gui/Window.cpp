#include "gui/Window.hpp"

#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/Button.hpp"
#include "gui/Component.hpp"

constexpr auto  TOP_BAR_WIDTH = 20.f;

namespace gui
{

Window::Window()
: killed_{false}
, focused_{false}
, style_(BasicStyleSheetFactory::create())
{   
    background_.setFillColor(style_.getInactiveWindowColor());
    background_.setOutlineColor(style_.getOutlineColor());
    background_.setOutlineThickness(style_.getOutlineThickness());

    auto close_button = std::make_unique<gui::Button>("X");
    close_button->setPosition(sf::Vector2f(0.f, 0.f), gui::Alignment::LEFT);
    close_button->setSize(sf::Vector2f(20.f, TOP_BAR_WIDTH));
    close_button->onClick([window = this](){window->close();});
    this->addChild(std::move(close_button));

    top_bar_.setOutlineColor(style_.getOutlineColor());
    top_bar_.setOutlineThickness(style_.getOutlineThickness());
}

void Window::setSize(const sf::Vector2f& size)
{
    auto top_bar_size = size;
    top_bar_size.y = TOP_BAR_WIDTH;

    auto background_size = size;
    background_size.y = size.y - TOP_BAR_WIDTH;

    top_bar_.setSize(top_bar_size);
    background_.setSize(background_size);

    Component::setSize(size);
}

void Window::setPosition(const sf::Vector2f& position, const Alignment alignment)
{
    Component::setPosition(position, alignment);
    top_bar_.setPosition(Component::getGlobalPosition());
    auto background_position = Component::getGlobalPosition();
    background_position.y += TOP_BAR_WIDTH;
    background_.setPosition(background_position);
}

bool Window::isInside(const sf::Vector2f point)
{
    return bounds_.contains(point);
}

void Window::onRender(sf::RenderWindow& renderWindow)
{
    renderWindow.draw(background_);
    renderWindow.draw(top_bar_);
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
    background_.setFillColor(style_.getWindowColor());
    top_bar_.setFillColor(style_.getTopBarWindowColor());
}

void Window::defocus()
{
    focused_ = false;
    background_.setFillColor(style_.getInactiveWindowColor());
    top_bar_.setFillColor(style_.getInactiveTopBarWindowColor());
}

bool Window::hasFocus()
{
    return focused_;
}

}  // namespace gui