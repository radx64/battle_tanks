#pragma once

#include <memory>
#include <vector>

#include "engine/Particle.hpp"

namespace engine::gfx { class RenderTarget; }

namespace engine
{

class ParticleSystem
{
public:
    ParticleSystem();

    void add(std::unique_ptr<Particle> particle);
    void draw(engine::gfx::RenderTarget& renderTarget);
    void clear();
    void update(float time_step);

protected:
    std::vector<std::unique_ptr<Particle>> active_particles_;
    std::vector<std::unique_ptr<Particle>> particles_to_spawn_;
};

}  // namespace engine
