#pragma once


#include "engine/Particle.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Sprite.hpp"

namespace game::particle
{

class Explosion : public engine::Particle
{
public:
    Explosion(float x, float y);
    virtual ~Explosion() = default;

    void draw(engine::gfx::RenderTarget& render_window) override;

protected:
    void onUpdate(float time_step) override;
    float lifetime_;

    engine::gfx::Sprite sprite_;
};

}  // namespace game::particle
