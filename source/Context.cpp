#include "Context.hpp"

Particles& Context::getParticles()
{
    return *particles_;
}
void Context::setParticles(Particles* particles)
{
    particles_ = particles;
}

Particles* Context::particles_{nullptr};
