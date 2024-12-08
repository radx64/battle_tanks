#include "engine/Context.hpp"

namespace engine 
{

engine::ParticleSystem& Context::getParticleSystem()
{
    return *particleSystem_;
}
void Context::setParticleSystem(engine::ParticleSystem* particles)
{
    particleSystem_ = particles;
}

engine::Scene& Context::getScene()
{
    return *scene_;
}

void Context::setScene(engine::Scene* scene)
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

engine::TimerService& Context::getTimerService()
{
    return *timerService_;
}

void Context::setTimerService(engine::TimerService* timerService)
{
    timerService_ = timerService;
}

engine::ParticleSystem* Context::particleSystem_{nullptr};
engine::Scene* Context::scene_{nullptr};
engine::Camera* Context::camera_{nullptr};
engine::TimerService* Context::timerService_{nullptr};

}  // namespace engine
