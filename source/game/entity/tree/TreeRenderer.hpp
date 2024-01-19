#ifndef GAME_ENTITY_TREE_RENDERER_HPP_
#define GAME_ENTITY_TREE_RENDERER_HPP_

#include "engine/Renderer.hpp"

namespace game::entity { class Tree; }

namespace game::entity
{
class TreeRenderer : public engine::Renderer
{
public:
    TreeRenderer(Tree* tree, sf::Texture& treeBody);
    void draw(sf::RenderWindow& renderWindow);

protected:
    Tree* tree_;
    sf::Sprite treeSprite_;
};


}  // namespace game::entity 

#endif  // GAME_ENTITY_TREE_RENDERER_HPP_