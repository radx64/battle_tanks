#include "game/Context.hpp"

namespace game 
{

graphics::Particles& Context::getParticles()
{
    return *particles_;
}
void Context::setParticles(graphics::Particles* particles)
{
    particles_ = particles;
}

graphics::Particles* Context::particles_{nullptr};

}  // namespace game
