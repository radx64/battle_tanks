#ifndef GAME_BARREL_HPP_
#define GAME_BARREL_HPP_

#include <SFML/Graphics.hpp>

#include "game/GameObject.hpp"

namespace game
{

class Barrel : public GameObject
{
public:
    Barrel(float x, float y, sf::Texture& barrelBody);
    virtual ~Barrel() = default;
};



}  // namespace game

#endif  // GAME_BARREL_HPP_