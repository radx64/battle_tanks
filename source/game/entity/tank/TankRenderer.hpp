#pragma once

#include "engine/Renderer.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Sprite.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity { class Tank; }

namespace game::entity
{
class TankRenderer : public engine::Renderer
{
public:
    TankRenderer(Tank* tank, engine::gfx::Texture& tankBody);
    void draw(engine::gfx::RenderTarget& render_window);
    void drawDebugInfo(engine::gfx::RenderTarget& render_window);

protected:
    Tank* tank_;
    engine::gfx::Sprite tankSprite_;
};

}  // namespace game::entity 
