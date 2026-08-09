#include "engine/Context.hpp"

namespace engine 
{

engine::ParticleSystem& Context::getParticleSystem()
{
    return *particle_system_;
}
void Context::setParticleSystem(engine::ParticleSystem* particles)
{
    particle_system_ = particles;
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
    return *timer_service_;
}

void Context::setTimerService(engine::TimerService* timerService)
{
    timer_service_ = timerService;
}

engine::ParticleSystem* Context::particle_system_{nullptr};
engine::Scene* Context::scene_{nullptr};
engine::Camera* Context::camera_{nullptr};
engine::TimerService* Context::timer_service_{nullptr};

}  // namespace engine
