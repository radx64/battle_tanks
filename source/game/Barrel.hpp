#ifndef GAME_BARREL_HPP_
#define GAME_BARREL_HPP_

#include <SFML/Graphics.hpp>

#include "game/IRenderable.hpp"
#include "game/RigidBody.hpp"

namespace game
{

class Barrel : public IRenderable, public RigidBody
{
public:
    Barrel(uint32_t id, float x, float y, sf::Texture& barrelBody);
    virtual ~Barrel() = default;
    void draw(sf::RenderWindow& renderWindow) override;

protected:
    sf::Sprite sprite_;
};

}  // namespace game

#endif  // GAME_BARREL_HPP_