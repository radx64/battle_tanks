#ifndef GAME_CONTEXT_HPP_
#define GAME_CONTEXT_HPP_

namespace graphics { class ParticleSystem; }
namespace game { class World; }

namespace game 
{
    
class Context
{
public:
    static graphics::ParticleSystem& getParticleSystem();
    static void setParticleSystem(graphics::ParticleSystem* particles);

    static game::World& getWorld();
    static void setWorld(game::World* world);

protected:
    static graphics::ParticleSystem* particleSystem_;
    static game::World* world_;
};

}  // namespace game

#endif  // GAME_CONTEXT_HPP_