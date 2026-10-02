#pragma once

#include "engine/Renderer.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Sprite.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity { class Bullet; }

namespace game::entity
{
class BulletRenderer : public engine::Renderer
{
public:
    BulletRenderer(Bullet* bullet, engine::gfx::Texture& bulletBody);
    void draw(engine::gfx::RenderTarget& render_window) override;

protected:
    Bullet* bullet_;
    engine::gfx::Sprite bulletSprite_;
};

}  // namespace game::entity 
