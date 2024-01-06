#ifndef GAME_TREE_HPP_
#define GAME_TREE_HPP_

#include <SFML/Graphics.hpp>

#include "game/Renderer.hpp"
#include "game/RigidBody.hpp"
#include "game/TreeRenderer.hpp"
namespace game
{

class Tree : public Renderer, public RigidBody
{
public:
    Tree(float x, float y, float height, sf::Texture& treeBody, float treeTrunkRadius);
    virtual ~Tree() = default;
    void draw(sf::RenderWindow& renderWindow) override;

    float height_;

protected:
    TreeRenderer renderer_;
};

}  // namespace game

#endif  // GAME_TREE_HPP_