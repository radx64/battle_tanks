#include "game/Context.hpp"

namespace game 
{

graphics::ParticleSystem& Context::getParticleSystem()
{
    return *particleSystem_;
}
void Context::setParticleSystem(graphics::ParticleSystem* particles)
{
    particleSystem_ = particles;
}

game::World& Context::getWorld()
{
    return *world_;
}

void Context::setWorld(game::World* world)
{
    world_ = world;
}

game::Camera& Context::getCamera()
{
    return *camera_;
}

void Context::setCamera(game::Camera* camera)
{
    camera_ = camera;
}

graphics::ParticleSystem* Context::particleSystem_{nullptr};
game::World* Context::world_{nullptr};
game::Camera* Context::camera_{nullptr};

}  // namespace game
