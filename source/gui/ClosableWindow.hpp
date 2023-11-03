#ifndef GUI_CLOSABLE_WINDOW_HPP_
#define GUI_CLOSABLE_WINDOW_HPP_

#include <SFML/Graphics.hpp>

#include "gui/Window.hpp"

namespace gui
{


// Consider decorator pattern in the future for easy decorator composition
class ClosableWindow : public Window
{
public:
    ClosableWindow();
    void onRender(sf::RenderWindow& renderWindow) override;
    void setPosition(const sf::Vector2f& position, const Alignment alignment) override;

protected:
    sf::RectangleShape top_bar_;
};

}  // namespace gui

#endif  // GUI_CLOSABLE_WINDOW_HPP_