#ifndef GAME_BARREL_HPP_
#define GAME_BARREL_HPP_

#include <SFML/Graphics.hpp>

#include "game/BarrelRenderer.hpp"
#include "game/Renderer.hpp"
#include "game/RigidBody.hpp"


namespace game
{

class Barrel : public Renderer, public RigidBody
{
public:
    Barrel(float x, float y, sf::Texture& barrelBody);
    virtual ~Barrel() = default;
    void draw(sf::RenderWindow& renderWindow) override;

protected:
    BarrelRenderer renderer_;
};

}  // namespace game

#endif  // GAME_BARREL_HPP_