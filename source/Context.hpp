#ifndef CONTEXT_HPP_
#define CONTEST_HPP_

class Particles;

class Context
{
public:
    static Particles& getParticles()
    {
        return *particles_;
    }
    static void setParticles(Particles* particles)
    {
        particles_ = particles;
    }

protected:
    static Particles* particles_;
};

#endif  // CONTEST_HPP_