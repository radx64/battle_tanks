#pragma once

#include <functional>
#include <memory>
#include <string_view>
#include <optional>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui { class Label; }

namespace gui
{

class ButtonBase : public Component
{
public:

    //TODO: cleanup some of this implementation
    // eg. stop using backgroundShape_ rectangle and use only sprite
    // focus texture is not here yet

    // static std::unique_ptr<Button> create(
    //     const std::optional<const std::string_view&>& text,
    //     const std::optional<sf::Texture&>& icon);
    
    void onSizeChange() override;
    void onPositionChange() override;

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
    ButtonBase();
    sf::RectangleShape backgroundShape_;
    sf::Sprite background_;

    const sf::Texture& hoverTexture_;
    const sf::Texture& normalTexture_;
    const sf::Texture& pressedTexture_;

    std::function<void()> onClick_;
};

class TextButton : public ButtonBase
{
public:
    static std::unique_ptr<TextButton> create(const std::string_view& text);

    void setText(const std::string_view& text);
    
protected:
    TextButton(const std::string_view& text);
    void onSizeChange() override;
    
    gui::Label* text_;
};

class IconButton : public ButtonBase
{  
public:
    static std::unique_ptr<IconButton> create(const sf::Texture& icon);

    void setIcon(const sf::Texture& icon);
    void onRender(sf::RenderTexture& renderTexture) override;

protected:
    IconButton(const sf::Texture& icon);
    void onSizeChange() override;
    void onPositionChange() override;

    sf::Sprite icon_;
};

}  // namespace gui
