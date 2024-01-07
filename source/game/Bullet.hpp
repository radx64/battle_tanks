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
};

}  // namespace game

#endif  // GAME_BULLET_HPP_