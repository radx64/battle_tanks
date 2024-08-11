#pragma once
#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/Text.hpp"
#include "gui/TextCursor.hpp"

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
    EventStatus on(const event::TextEntered& textEntered) override;

    void onFocus() override;
    void onFocusLost() override; 

    void updateTextVisbleArea();

    sf::RectangleShape background_;
    gui::Text text_;
    gui::TextCursor text_cursor_;
    uint32_t max_length_;
};

}  // namespace gui

