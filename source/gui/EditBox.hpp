#pragma once
#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/Text.hpp"

namespace gui
{

class EditBox : public Component
{
public:
    EditBox();

    std::string getText();

protected:
    void onRender(sf::RenderTexture& renderTexture) override;
    void onSizeChange() override;
    void onPositionChange() override;

    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;

    EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressed) override;
    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleased) override;

    void onFocus() override;
    void onFocusLost() override; 

    sf::RectangleShape background_;
    sf::RectangleShape cursor_;
    gui::Text text_;
};

}  // namespace gui

