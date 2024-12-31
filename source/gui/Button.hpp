#pragma once

#include <functional>
#include <string_view>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui { class Label; }

namespace gui
{

class Button : public Component
{
public:
    Button(const std::string_view& text);

    void onSizeChange() override;
    void onPositionChange() override;
    void setText(const std::string_view& text);
    void onRender(sf::RenderTexture& renderTexture) override;
    void onClick(std::function<void()> onClickCallback);

    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseEntered& mouseEnteredEvent) override;
    EventStatus on(const event::MouseLeft& mouseLeftEvent) override;

    EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressedEvent) override;
    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;

    EventStatus on(const event::FocusLost&) override;
    EventStatus on(const event::FocusGained&) override;

protected:
    sf::RectangleShape backgroundShape_;
    gui::Label* text_;
    bool isButtonHoldDown_;
    std::function<void()> onClick_;
};

}  // namespace gui
