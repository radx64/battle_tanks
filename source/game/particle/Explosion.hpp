#pragma once

#include <SFML/Graphics.hpp>

#include "engine/Particle.hpp"

namespace game::particle
{

class Explosion : public engine::Particle
{
public:
    Explosion(float x, float y);
    virtual ~Explosion() = default;

    void draw(sf::RenderWindow& renderWindow) override;

protected:
    void onUpdate(float timeStep) override;
    float lifetime_;

    sf::Sprite sprite_;
};

}  // namespace game::particle

