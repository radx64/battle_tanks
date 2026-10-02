#pragma once

#include "engine/Renderer.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Sprite.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity { class Barrel; }

namespace game::entity
{
class BarrelRenderer : public engine::Renderer
{
public:
    BarrelRenderer(Barrel* barrel, engine::gfx::Texture& barrelBody);
    void draw(engine::gfx::RenderTarget& render_window) override;

protected:
    Barrel* barrel_;
    engine::gfx::Sprite barrelSprite_;
};

}  // namespace game::entity 
