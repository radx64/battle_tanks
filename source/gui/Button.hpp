#pragma once


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

    void onSizeChange() override;
    void onPositionChange() override;
    void setText(const sf::String& text);
    void onRender(sf::RenderTexture& renderTexture) override;
    void onClick(std::function<void()> onClickCallback);

    EventStatus on(const event::MouseButtonPressed& mousePressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseEntered& mouseEnteredEvent) override;
    EventStatus on(const event::MouseLeft& mouseLeftEvent) override;

protected:
    sf::RectangleShape background_;
    gui::Label* text_;
    bool isButtonHoldDown_;
    std::function<void()> on_click_;
};

}  // namespace gui

