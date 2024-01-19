#ifndef GAME_ENTITY_TREE_TREE_HPP_
#define GAME_ENTITY_TREE_TREE_HPP_

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

#endif  // GAME_ENTITY_TREE_TREE_HPP_