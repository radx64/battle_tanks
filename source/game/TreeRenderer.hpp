#ifndef GAME_TREE_RENDERER_HPP_
#define GAME_TREE_RENDERER_HPP_

#include "game/Renderer.hpp"

namespace game { class Tree; }

namespace game
{
class TreeRenderer : public Renderer
{
public:
    TreeRenderer(Tree* tree, sf::Texture& treeBody);
    void draw(sf::RenderWindow& renderWindow);

protected:
    Tree* tree_;
    sf::Sprite treeSprite_;
};


}  // namespace game 

#endif  // GAME_TREE_RENDERER_HPP_