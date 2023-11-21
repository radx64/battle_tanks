#ifndef GUI_BUTTON_HPP_
#define GUI_BUTTON_HPP_

#include <functional>
#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui { class Label; }

namespace gui
{

class Button : public Component
{
public:
    Button(const std::string_view& text);
    ~Button();

    void setSize(const sf::Vector2f& size) override;
    void setText(const sf::String& text);
    void onRender(sf::RenderWindow& renderWindow) override;
    void onClick(std::function<void()> onClickCallback);

    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;
    EventStatus on(const event::MouseButtonPressed& mousePressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;

protected:
    sf::RectangleShape background_;
    gui::Label* text_;
    bool isButtonHoldDown_;
    std::function<void()> on_click_;
};

}  // namespace gui

#endif  // GUI_BUTTON_HPP_