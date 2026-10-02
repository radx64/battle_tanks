#pragma once

#include <memory>
#include <functional>

#include "engine/gfx/Texture.hpp"
#include "engine/gfx/Sprite.hpp"
#include "engine/gui/Widget.hpp"


namespace engine::gui
{

class Checkbox : public Widget
{
public:
    static std::unique_ptr<Checkbox> create(const bool isChecked);

    void onRender(engine::gfx::RenderTarget& renderTarget) override;
    void onStateChange(std::function<void(bool)> onClickCallback);
    void setState(const bool checked);
    bool isChecked() const;
    
protected:
    Checkbox(const bool isChecked,
        const engine::gfx::Texture& normalTexture,
        const engine::gfx::Texture& hoverTexture,
        const engine::gfx::Texture& focusTexture,
        const engine::gfx::Texture& checkedTexture);

    Checkbox(const bool isChecked);
    
    void updateTexture();
    void recalculateSpritesPositions();

    enum class State
    {
        Normal,
        Hover, // TODO add disabled in the future
    };

    EventStatus on(const engine::events::KeyReleased& keyboardKeyReleasedEvent) override;
    EventStatus on(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const engine::gui::events::MouseEntered& mouseEnteredEvent) override;
    EventStatus on(const engine::gui::events::MouseLeft& mouseLeftEvent) override;

    EventStatus on(const engine::gui::events::FocusLost&) override;
    EventStatus on(const engine::gui::events::FocusGained&) override;

    void onPositionChange() override;
    void onSizeChange() override;

    bool isChecked_ = false;
    std::function<void(bool)> onClick_;
    State state_;
    engine::gfx::Sprite background_;
    engine::gfx::Sprite tick_;
    const engine::gfx::Texture& hoverTexture_;
    const engine::gfx::Texture& focusTexture_;
    const engine::gfx::Texture& normalTexture_;
    const engine::gfx::Texture& checkedTexture_;
};

}  // namespace engine::gui
