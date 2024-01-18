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

game::Scene& Context::getScene()
{
    return *scene_;
}

void Context::setScene(game::Scene* scene)
{
    scene_ = scene;
}

engine::Camera& Context::getCamera()
{
    return *camera_;
}

void Context::setCamera(engine::Camera* camera)
{
    camera_ = camera;
}

graphics::ParticleSystem* Context::particleSystem_{nullptr};
game::Scene* Context::scene_{nullptr};
engine::Camera* Context::camera_{nullptr};

}  // namespace game
