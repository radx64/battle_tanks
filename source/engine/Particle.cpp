#include "engine/Particle.hpp"

#include <cmath>

namespace engine
{

Particle::Particle()
: position_{0.0f, 0.0f}
, velocity_{0.0f, 0.0f}
, rotation_ {0.0f}
, angularVelocity_ {0.0}
, isDead_{false}
{}

void Particle::draw(sf::RenderWindow& render_window)
{
    (void)render_window;
}

void Particle::update(float time_step)
{
    onUpdate(time_step);
    position_ += velocity_ * time_step;
    rotation_ += angularVelocity_ * time_step;
    rotation_ = std::fmod(rotation_, 360.f);
}

void Particle::onUpdate(float time_step)
{
    (void) time_step;
}

bool Particle::isDead()
{
    return isDead_;
}
void Particle::kill()
{
    isDead_ = true;
}

}  // namespace engine
