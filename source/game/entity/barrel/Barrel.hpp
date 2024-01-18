#ifndef GAME_ENTITY_BARREL_BARREL_HPP_
#define GAME_ENTITY_BARREL_BARREL_HPP_

#include <SFML/Graphics.hpp>

#include "game/GameObject.hpp"

namespace game::entity
{

class Barrel : public GameObject
{
public:
    Barrel(float x, float y, sf::Texture& barrelBody);
    virtual ~Barrel() = default;
};



}  // namespace game::entity

#endif  // GAME_ENTITY_BARREL_BARREL_HPP_