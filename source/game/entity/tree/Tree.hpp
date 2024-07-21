#pragma once


#include <SFML/Graphics.hpp>

#include "engine/GameObject.hpp"

namespace game::entity
{

class Tree : public engine::GameObject
{
public:
    Tree(float x, float y, float height, sf::Texture& treeBody, float treeTrunkRadius);
    virtual ~Tree() = default;

    float height_;
};

}  // namespace game::entity

