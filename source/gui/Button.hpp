#pragma once

#include <functional>
#include <memory>
#include <string_view>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui { class Label; }

namespace gui
{

class Button : public Component
{
public:
    static std::unique_ptr<Button> create(const std::string_view& text);
    
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
    Button(const std::string_view& text);
    sf::RectangleShape backgroundShape_;
    gui::Label* text_;
    std::function<void()> onClick_;
};

class ButtonWithIcon : public Button
{
public:
    static std::unique_ptr<ButtonWithIcon> create(const std::string_view& text, const std::string_view& icon);
    
    void onRender(sf::RenderTexture& renderTexture) override;
protected:
    ButtonWithIcon(const std::string_view& text, const std::string_view& icon);
    sf::Sprite icon_;
};

}  // namespace gui
