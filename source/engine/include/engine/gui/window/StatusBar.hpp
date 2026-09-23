#pragma once

#include <SFML/Graphics.hpp>

#include "engine/gfx/Texture.hpp"
#include "engine/gfx/Sprite.hpp"

#include "engine/gui/Widget.hpp"

namespace engine::gui::window
{

//TODO: add statusbar inset texture

class StatusBar : public Widget
{
public:
    StatusBar();

    void enable();
    void disable();
    bool isInsideResizeGadget(const engine::Vec2f point);

protected:
    void onRender(engine::gfx::RenderTarget& renderTarget) override;
    void onSizeChange() override;
    void onPositionChange() override;

    engine::gfx::Sprite resize_gadget_;
    const engine::gfx::Texture& resize_gadget_texture_;
};

}  // namespace engine::gui::window
