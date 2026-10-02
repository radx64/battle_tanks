#pragma once

#include "engine/Renderer.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Sprite.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity { class Crate; }

namespace game::entity
{
class CrateRenderer : public engine::Renderer
{
public:
    CrateRenderer(Crate* crate, engine::gfx::Texture& crateBody);
    void draw(engine::gfx::RenderTarget& render_window) override;

protected:
    Crate* crate_;
    engine::gfx::Sprite crateSprite_;
};


}  // namespace game::entity 
