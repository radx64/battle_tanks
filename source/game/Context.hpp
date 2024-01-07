#ifndef GAME_CONTEXT_HPP_
#define GAME_CONTEXT_HPP_

namespace graphics { class Particles; }
namespace game { class World; }

namespace game 
{
    
class Context
{
public:
    static graphics::Particles& getParticles();
    static void setParticles(graphics::Particles* particles);

    static game::World& getWorld();
    static void setWorld(game::World* world);

protected:
    static graphics::Particles* particles_;
    static game::World* world_;
};

}  // namespace game

#endif  // GAME_CONTEXT_HPP_