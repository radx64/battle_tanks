#pragma once

#include <SFML/Graphics.hpp>

namespace engine
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
    float angularVelocity_;
    bool isDead_;
};

}  // namespace engine
