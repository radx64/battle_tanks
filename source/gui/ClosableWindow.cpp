#include "gui/ClosableWindow.hpp"

#include "gui/Button.hpp"

namespace gui
{

ClosableWindow::ClosableWindow(Component* parent) 
: Window(parent)
{
    auto close_button = new gui::Button(this, "X");
    close_button->setPosition(sf::Vector2f(5.f, 5.f), gui::Alignment::LEFT);
    close_button->setSize(sf::Vector2f(30.f, 30.f));
    close_button->onClick([window = this](){window->close();});
} 

}  // namespace gui