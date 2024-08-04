#pragma once

#include <SFML/Graphics.hpp>

#include <gui/Component.hpp>

namespace gui
{

class EditBox : public Component
{
public:
    EditBox();

    void onRender(sf::RenderTexture& renderTexture) override;

protected:
    void onSizeChange() override;
    void onPositionChange() override;

    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;

    EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressed) override;
    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleased) override;

    sf::RectangleShape background_;
    sf::RectangleShape cursor_;
    sf::Text text_;
};

}  // namespace gui

