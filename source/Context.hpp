#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

class Particles;

class Context
{
public:
    static Particles& getParticles();
    static void setParticles(Particles* particles);

protected:
    static Particles* particles_;
};

#endif  // CONTEXT_HPP_