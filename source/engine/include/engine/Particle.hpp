#pragma once

#include "engine/Vector2.hpp"

namespace engine::gfx { class RenderTarget; }

namespace engine
{

class Particle
{
public:
    Particle();
    virtual ~Particle() = default;

    virtual void draw(engine::gfx::RenderTarget& renderTarget);
    void update(float time_step);
    bool isDead();
    void kill();

protected:
    virtual void onUpdate(float time_step);

    // TODO: add Transform class here
    engine::Vec2f position_;
    engine::Vec2f velocity_;
    float rotation_;
    float angularVelocity_;
    bool isDead_;
};

}  // namespace engine
