#include "engine/Particle.hpp"

#include <cmath>

namespace engine
{

Particle::Particle()
: position_{0.0f, 0.0f}
, velocity_{0.0f, 0.0f}
, rotation_ {0.0f}
, angular_velocity_ {0.0}
, is_dead_{false}
{}

void Particle::draw(sf::RenderWindow& renderWindow)
{
    (void) renderWindow;
}

void Particle::update(float timeStep)
{
    onUpdate(timeStep);
    position_ += velocity_ * timeStep;
    rotation_ += angular_velocity_ * timeStep;
    rotation_ = std::fmod(rotation_, 360.f);
}

void Particle::onUpdate(float timeStep)
{
    (void) timeStep;
}

bool Particle::isDead()
{
    return is_dead_;
}
void Particle::kill()
{
    is_dead_ = true;
}

}  // namespace engine
