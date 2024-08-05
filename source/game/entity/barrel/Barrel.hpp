#pragma once

#include <SFML/Graphics.hpp>

#include "engine/GameObject.hpp"

namespace game::entity
{

class Barrel : public engine::GameObject
{
public:
    Barrel(float x, float y, sf::Texture& barrelBody);
    virtual ~Barrel() = default;
};



}  // namespace game::entity

