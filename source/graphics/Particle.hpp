#ifndef GRAPHICS_PARTICLE_HPP_
#define GRAPHICS_PARTICLE_HPP_

#include <SFML/Graphics.hpp>

namespace graphics
{

class Particle
{
public:
    Particle();
    virtual ~Particle() = default;

    virtual void draw(sf::RenderWindow& renderWindow);
    void update(float timeStep);
    bool isDead();
    void kill();

protected:
    virtual void onUpdate(float timeStep); // This method is used to define custom particle behaviour

    sf::Vector2f position_;
    sf::Vector2f velocity_;
    float rotation_;
    float angular_velocity_;
    bool is_dead_;
};

}  // namespace graphics

#endif  // GRAPHICS_PARTICLE_HPP_