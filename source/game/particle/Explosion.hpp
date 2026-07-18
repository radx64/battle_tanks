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

    void draw(sf::RenderWindow& render_window) override;

protected:
    void onUpdate(float time_step) override;
    float lifetime_;

    sf::Sprite sprite_;
};

}  // namespace game::particle
