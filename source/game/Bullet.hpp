#ifndef GAME_BULLET_HPP_
#define GAME_BULLET_HPP_

#include <SFML/Graphics.hpp>

#include "game/GameObject.hpp"

namespace game
{

class Bullet : public GameObject
{
public:
    Bullet(float x, float y, float angle, float speed, sf::Texture& bulletBody);
    virtual ~Bullet() = default;

protected:
    void onUpdate(game::World& world, float timeStep) override;

    float lifetime_;
};

}  // namespace game

#endif  // GAME_BULLET_HPP_