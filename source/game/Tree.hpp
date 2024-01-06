#ifndef GAME_TREE_HPP_
#define GAME_TREE_HPP_

#include <SFML/Graphics.hpp>

#include "game/GameObject.hpp"
namespace game
{

class Tree : public GameObject
{
public:
    Tree(float x, float y, float height, sf::Texture& treeBody, float treeTrunkRadius);
    virtual ~Tree() = default;

    float height_;
};

}  // namespace game

#endif  // GAME_TREE_HPP_