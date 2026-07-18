#pragma once

#include <SFML/Graphics.hpp>

namespace engine
{

class Particle
{
public:
    Particle();
    virtual ~Particle() = default;

    virtual void draw(sf::RenderWindow& render_window);
    void update(float time_step);
    bool isDead();
    void kill();

protected:
    virtual void onUpdate(float time_step); // This method is used to define custom particle behaviour

    // TODO: add Transform class here
    sf::Vector2f position_;
    sf::Vector2f velocity_;
    float rotation_;
    float angularVelocity_;
    bool isDead_;
};

}  // namespace engine
