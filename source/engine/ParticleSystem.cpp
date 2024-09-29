#include "engine/ParticleSystem.hpp"

#include "engine/Particle.hpp"

namespace engine
{

ParticleSystem::ParticleSystem()
: activeParticles_{}
, particlesToSpawn_{}
{

}

void ParticleSystem::add(std::unique_ptr<Particle> particle)
{
    particlesToSpawn_.push_back(std::move(particle));
}

void ParticleSystem::draw(sf::RenderWindow& renderWindow)
{
    for (const auto& particle : activeParticles_)
    {
        particle->draw(renderWindow);
    }
}

void ParticleSystem::clear()
{
    particlesToSpawn_.clear();
    activeParticles_.clear();
}

// TODO this update method is somewhat simmilar to scene game objects handling
// Consider making this common
void ParticleSystem::update(float timeStep)
{
    for (auto& particle : activeParticles_)
    {
        particle->update(timeStep);
    }

    activeParticles_.erase(
        std::remove_if(activeParticles_.begin(), activeParticles_.end(),
            [](auto& particle){ return particle->isDead();}),
        activeParticles_.end());

    if (not particlesToSpawn_.empty())
    {
        activeParticles_.insert(activeParticles_.end(), std::make_move_iterator(particlesToSpawn_.begin()), std::make_move_iterator(particlesToSpawn_.end()));
        particlesToSpawn_.clear();
    }
}

}  // namespace engine
