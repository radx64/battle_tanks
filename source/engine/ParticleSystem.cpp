#include "engine/ParticleSystem.hpp"

#include "engine/Particle.hpp"

namespace engine
{

ParticleSystem::ParticleSystem()
: active_particles_{}
, particles_to_spawn_{}
{

}

void ParticleSystem::add(std::unique_ptr<Particle> particle)
{
    particles_to_spawn_.push_back(std::move(particle));
}

void ParticleSystem::draw(sf::RenderWindow& renderWindow)
{
    for (const auto& particle : active_particles_)
    {
        particle->draw(renderWindow);
    }
}

void ParticleSystem::clear()
{
    particles_to_spawn_.clear();
    active_particles_.clear();
}

// TODO this update method is somewhat simmilar to scene game objects handling
// Consider making this common
void ParticleSystem::update(float timeStep)
{
    for (auto& particle : active_particles_)
    {
        particle->update(timeStep);
    }

    active_particles_.erase(
        std::remove_if(active_particles_.begin(), active_particles_.end(),
            [](auto& particle){ return particle->isDead();}),
        active_particles_.end());

    if (not particles_to_spawn_.empty())
    {
        active_particles_.insert(active_particles_.end(), std::make_move_iterator(particles_to_spawn_.begin()), std::make_move_iterator(particles_to_spawn_.end()));
        particles_to_spawn_.clear();
    }
}

}  // namespace engine
