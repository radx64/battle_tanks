#include "game/TreeRenderer.hpp"
#include "game/Tree.hpp"

namespace game
{
TreeRenderer::TreeRenderer(Tree* tree, sf::Texture& treeBody)
: tree_(tree)
{
    treeSprite_.setTexture(treeBody);
    sf::Vector2u texture_body_size = treeBody.getSize();
    auto middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    treeSprite_.setOrigin(middle_point);     
}

void TreeRenderer::draw(sf::RenderWindow& renderWindow)
{
    treeSprite_.setRotation(tree_->rotation_);
    treeSprite_.setColor(sf::Color(10, 10, 10, 127));
    treeSprite_.setPosition(tree_->x_ + tree_->height_, tree_->y_+ tree_->height_);
    renderWindow.draw(treeSprite_);

    treeSprite_.setColor(sf::Color(255, 255, 255, 255));
    treeSprite_.setPosition(tree_->x_, tree_->y_);
    renderWindow.draw(treeSprite_);
}

}  // namespace game