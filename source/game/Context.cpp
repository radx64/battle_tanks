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

game::World& Context::getWorld()
{
    return *world_;
}

void Context::setWorld(game::World* world)
{
    world_ = world;
}

graphics::Particles* Context::particles_{nullptr};
game::World* Context::world_{nullptr};

}  // namespace game
