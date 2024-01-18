#ifndef GAME_CONTEXT_HPP_
#define GAME_CONTEXT_HPP_

namespace graphics { class ParticleSystem; }
namespace game { class Scene; }
namespace engine { class Camera; }
namespace game 
{
    
class Context
{
public:
    static graphics::ParticleSystem& getParticleSystem();
    static void setParticleSystem(graphics::ParticleSystem* particles);

    static game::Scene& getScene();
    static void setScene(game::Scene* scene);

    static engine::Camera& getCamera();
    static void setCamera(engine::Camera* camera);

protected:
    static graphics::ParticleSystem* particleSystem_;
    static game::Scene* scene_;
    static engine::Camera* camera_;
};

}  // namespace game

#endif  // GAME_CONTEXT_HPP_