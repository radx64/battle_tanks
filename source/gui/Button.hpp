#pragma once

#include <functional>
#include <memory>
#include <string_view>
#include <optional>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/FramedSprite.hpp"
#include "gui/Image.hpp"

namespace gui { class Label; }

namespace gui
{

class ButtonBase : public Component
{
public:
    void onRender(sf::RenderTexture& renderTexture) override;
    void onClick(std::function<void()> onClickCallback);
    
protected:
    ButtonBase();
    void updateTexture();
    
    enum class State
    {
        Normal,
        Hover,
        Pressed,    // TODO add disabled in the future
    };
    

    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseEntered& mouseEnteredEvent) override;
    EventStatus on(const event::MouseLeft& mouseLeftEvent) override;
    
    EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressedEvent) override;
    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    
    EventStatus on(const event::FocusLost&) override;
    EventStatus on(const event::FocusGained&) override;

    void onSizeChange() override;
    void onPositionChange() override;

    gui::FramedSprite background_;

    const sf::Texture& hoverTexture_;
    const sf::Texture& focusTexture_;
    const sf::Texture& normalTexture_;
    const sf::Texture& pressedTexture_;

    State state_;

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
    IconButton(const sf::Texture& texture);
    void onSizeChange() override;
    void onPositionChange() override;

    Image* icon_;
    Component* layout_;
};

}  // namespace gui
