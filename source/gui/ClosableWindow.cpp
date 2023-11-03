#include "gui/ClosableWindow.hpp"

#include "gui/Button.hpp"

namespace gui
{

ClosableWindow::ClosableWindow() 
{
    auto close_button = std::make_unique<gui::Button>("X");
    close_button->setPosition(sf::Vector2f(0.f, -20.f), gui::Alignment::LEFT);
    close_button->setSize(sf::Vector2f(20.f, 20.f));
    close_button->onClick([window = this](){window->close();});
    this->addChild(std::move(close_button));

    //FIXME fix styling for top bar
    top_bar_.setFillColor(style_.getInactiveWindowColor());
    top_bar_.setOutlineColor(style_.getOutlineColor());
    top_bar_.setOutlineThickness(style_.getOutlineThickness());
} 

void ClosableWindow::setPosition(const sf::Vector2f& position, const Alignment alignment)
{
    Window::setPosition(position, alignment);
    top_bar_.setPosition(getGlobalPosition() - sf::Vector2f(0.f, 20.f));
    top_bar_.setSize(sf::Vector2f(getWidth()-1.0f, 20.f));
}


void ClosableWindow::onRender(sf::RenderWindow& renderWindow)
{
    Window::onRender(renderWindow);
    renderWindow.draw(top_bar_);
}

}  // namespace gui