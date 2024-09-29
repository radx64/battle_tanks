#pragma once

#include <memory>
#include <vector>

#include "engine/Particle.hpp"

namespace sf {class RenderWindow;}

namespace engine
{

class ParticleSystem
{
public:
    ParticleSystem();

    void add(std::unique_ptr<Particle> particle);
    void draw(sf::RenderWindow& renderWindow);
    void clear();
    void update(float timeStep);

protected:
    std::vector<std::unique_ptr<Particle>> activeParticles_;
    std::vector<std::unique_ptr<Particle>> particlesToSpawn_;
};

}  // namespace engine
