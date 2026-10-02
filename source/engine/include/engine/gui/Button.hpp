#pragma once

#include <functional>
#include <memory>
#include <string_view>


#include "engine/gfx/Texture.hpp"

#include "engine/gui/FramedSprite.hpp"
#include "engine/gui/Image.hpp"
#include "engine/gui/Widget.hpp"
#include "engine/gui/WidgetState.hpp"
#include "engine/gui/style/Button.hpp"

namespace engine::gui { class Label; }
namespace engine::gui::style { class Sheet; }

namespace engine::gui
{

class ButtonBase : public Widget
{
public:
    void onRender(engine::gfx::RenderTarget& renderTarget) override;
    void onClick(std::function<void()> onClickCallback);
    void onMouseEnter(std::function<void()> onMouseEnterCallback);
    
protected:
    ButtonBase();
    ButtonBase(const style::Button& style);
    void updateTexture();
    
    EventStatus on(const engine::events::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const engine::gui::events::MouseEntered& mouseEnteredEvent) override;
    EventStatus on(const engine::gui::events::MouseLeft& mouseLeftEvent) override;
    EventStatus on(const engine::gui::events::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override;

    EventStatus on(const engine::events::KeyPressed& keyboardKeyPressedEvent) override;
    EventStatus on(const engine::events::KeyReleased& keyboardKeyReleasedEvent) override;
    
    EventStatus on(const engine::gui::events::FocusLost&) override;
    EventStatus on(const engine::gui::events::FocusGained&) override;

    void onSizeChange() override;
    void onPositionChange() override;

    EventStatus processLeftMouseClick();

    engine::gui::FramedSprite background_;
    const style::Button& style_;
    const engine::gfx::Texture& focusTexture_;

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
    
    engine::gui::Label* text_;
};

class IconButton : public ButtonBase
{  
public:
    static std::unique_ptr<IconButton> create(const engine::gfx::Texture& icon);
    static std::unique_ptr<IconButton> create(const engine::gfx::Texture& icon, const style::Button& style);

    void setIcon(const engine::gfx::Texture& icon);
    void onRender(engine::gfx::RenderTarget& renderTarget) override;

protected:
    IconButton(const engine::gfx::Texture& texture, const style::Button& style);
    void onSizeChange() override;
    void onPositionChange() override;

    Image* icon_;
    Widget* layout_;
};

}  // namespace engine::gui
