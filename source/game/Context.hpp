#ifndef GAME_CONTEXT_HPP_
#define GAME_CONTEXT_HPP_

class Particles;

class Context
{
public:
    static Particles& getParticles();
    static void setParticles(Particles* particles);

protected:
    static Particles* particles_;
};

#endif  // GAME_CONTEXT_HPP_