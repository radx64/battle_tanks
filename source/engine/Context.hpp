#pragma once

namespace engine { class ParticleSystem; }
namespace engine { class Scene; }
namespace engine { class Camera; }
namespace engine { class TimerService; }

namespace engine 
{
    
class Context
{
public:
    static engine::ParticleSystem& getParticleSystem();
    static void setParticleSystem(engine::ParticleSystem* particles);

    static engine::Scene& getScene();
    static void setScene(engine::Scene* scene);

    static engine::Camera& getCamera();
    static void setCamera(engine::Camera* camera);

    static engine::TimerService& getTimerService();
    static void setTimerService(engine::TimerService* timerService);

protected:
    static engine::ParticleSystem* particleSystem_;
    static engine::Scene* scene_;
    static engine::Camera* camera_;
    static engine::TimerService* timerService_;
};

}  // namespace engine
