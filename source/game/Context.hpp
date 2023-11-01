#ifndef GAME_CONTEXT_HPP_
#define GAME_CONTEXT_HPP_

namespace graphics{ class Particles; }

namespace game 
{
    
class Context
{
public:
    static graphics::Particles& getParticles();
    static void setParticles(graphics::Particles* particles);

protected:
    static graphics::Particles* particles_;
};

}  // namespace game

#endif  // GAME_CONTEXT_HPP_