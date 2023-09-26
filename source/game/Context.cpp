#include "game/Context.hpp"

graphics::Particles& Context::getParticles()
{
    return *particles_;
}
void Context::setParticles(graphics::Particles* particles)
{
    particles_ = particles;
}

graphics::Particles* Context::particles_{nullptr};
