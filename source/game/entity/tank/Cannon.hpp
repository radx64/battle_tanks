#pragma once

#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Sprite.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity 
{

class Cannon
{
public:
    Cannon(float x, float y, float rotation, engine::gfx::Texture& texture);
    void draw(engine::gfx::RenderTarget& render_window);
    void physics(float time_step);
    void setRotation(float rotation);
    void fire();
    bool canFire() const;

    float x_;
    float y_;
    float current_rotation_;
    float set_rotation_;
    float cooldown_;
    engine::gfx::Sprite sprite_;
};

}  // namespace game::entity
