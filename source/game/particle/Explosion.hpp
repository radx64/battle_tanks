#ifndef GAME_EXPLOSION_HPP_
#define GAME_EXPLOSION_HPP_

#include <SFML/Graphics.hpp>

#include "graphics/Particle.hpp"

namespace game::particle
{

class Explosion : public graphics::Particle
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

#endif  // GAME_EXPLOSION_HPP_