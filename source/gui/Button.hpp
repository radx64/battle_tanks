#pragma once

#include <functional>
#include <memory>
#include <string_view>

#include <SFML/Graphics.hpp>

#include "gui/FramedSprite.hpp"
#include "gui/Image.hpp"
#include "gui/Widget.hpp"
#include "gui/WidgetState.hpp"
#include "gui/style/Button.hpp"

namespace gui { class Label; }
namespace gui::style { class Sheet; }

namespace gui
{

class ButtonBase : public Widget
{
public:
    void onRender(sf::RenderTexture& renderTexture) override;
    void onClick(std::function<void()> onClickCallback);
    void onMouseEnter(std::function<void()> onMouseEnterCallback);
    
protected:
    ButtonBase();
    ButtonBase(const style::Button& style);
    void updateTexture();
    
    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseEntered& mouseEnteredEvent) override;
    EventStatus on(const event::MouseLeft& mouseLeftEvent) override;
    EventStatus on(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override;

    EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressedEvent) override;
    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    
    EventStatus on(const event::FocusLost&) override;
    EventStatus on(const event::FocusGained&) override;

    void onSizeChange() override;
    void onPositionChange() override;

    EventStatus processLeftMouseClick();

    gui::FramedSprite background_;
    const style::Button& style_;
    const sf::Texture& focusTexture_;

    WidgetState state_;

    std::function<void()> onClick_;
    std::function<void()> onMouseEnter_;
};

class TextButton : public ButtonBase
{
public:
    static std::unique_ptr<TextButton> create(const std::string_view& text);
    static std::unique_ptr<TextButton> create(const std::string_view& text, const style::Button& style);

    void setText(const std::string_view& text);
    
protected:
    TextButton(const std::string_view& text, const style::Button& style);
    void onSizeChange() override;
    
    gui::Label* text_;
};

class IconButton : public ButtonBase
{  
public:
    static std::unique_ptr<IconButton> create(const sf::Texture& icon);
    static std::unique_ptr<IconButton> create(const sf::Texture& icon, const style::Button& style);

    void setIcon(const sf::Texture& icon);
    void onRender(sf::RenderTexture& renderTexture) override;

protected:
    IconButton(const sf::Texture& texture, const style::Button& style);
    void onSizeChange() override;
    void onPositionChange() override;

    Image* icon_;
    Widget* layout_;
};

}  // namespace gui
