#include "graphics/ParticleSystem.hpp"

#include "graphics/Particle.hpp"

namespace graphics
{

constexpr auto TRACK_WIDTH = 6;
constexpr auto TRACK_HEIGHT = 2;
constexpr auto TRACK_IMPRINT_OPACITY = 50;
constexpr auto PARTICLE_LAYER_OPACITY = 90;

ParticleSystem::ParticleSystem()
: active_particles_{}
, particles_to_spawn_{}
{
    // FIXME: This layer should be resized alongside window resolution
    target_.create(1920, 1080);
}

// Temporary 'particle system' for drawing track imprints
void ParticleSystem::addTrackImprint(const int x, const int y, const float angle)
{
    sf::RectangleShape rectangle(sf::Vector2f(TRACK_HEIGHT, TRACK_WIDTH));
    rectangle.setPosition(sf::Vector2f(x, y));
    rectangle.setFillColor(sf::Color(0, 0, 0, TRACK_IMPRINT_OPACITY));
    rectangle.setRotation(angle);  
    target_.draw(rectangle);
    target_.display();
}

void ParticleSystem::addParticle(std::unique_ptr<Particle> particle)
{
    particles_to_spawn_.push_back(std::move(particle));
}

void ParticleSystem::drawTracks(sf::RenderWindow& renderWindow)
{
    sf::Sprite texture(target_.getTexture());
    texture.setColor(sf::Color(0, 0, 0, PARTICLE_LAYER_OPACITY));
    renderWindow.draw(texture);
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
    target_.clear(sf::Color(255,255,255,0));
    particles_to_spawn_.clear();
    active_particles_.clear();
}


// TODO this update method is somewhar simmilar to scene game objects handling
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

}  // namespace graphics
