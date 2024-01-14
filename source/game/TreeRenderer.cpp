#include "game/TreeRenderer.hpp"
#include "game/Tree.hpp"

#include "game/Camera.hpp"
#include "game/Context.hpp"
#include "math/Math.hpp"

constexpr float HEIGHT_COEFF = 0.01f;

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
    auto tree_rigid_body = tree_->getRigidBody();
    auto tree_position = sf::Vector2f{tree_rigid_body.x_, tree_rigid_body.y_};

    auto camera_postion = Context::getCamera().getPosition();
    auto camera_zoom_level = Context::getCamera().getZoom();

    auto difference = camera_postion - tree_position;

    auto tree_top_point = tree_position - (difference * tree_->height_ *  HEIGHT_COEFF * camera_zoom_level);

    const size_t tree_layers = 6;

    for (size_t layer = 0; layer < tree_layers; ++layer)
    {
        const float t = layer / static_cast<float>(tree_layers - 1);
        const float scale = math::lerp(1.0f, 0.2f, t);
        auto current_layer_point = math::lerp(tree_position, tree_top_point, t);

        treeSprite_.setScale(scale, scale);
        treeSprite_.setRotation(tree_rigid_body.rotation_ + layer * 30.f);
        treeSprite_.setColor(sf::Color(10, 10, 10, 127));
        treeSprite_.setPosition(current_layer_point + sf::Vector2f{tree_->height_ * scale, tree_->height_ * scale});
        renderWindow.draw(treeSprite_);

        treeSprite_.setColor(sf::Color(255, 255, 255, 255));
        treeSprite_.setPosition(current_layer_point);
        renderWindow.draw(treeSprite_);
    }
}

}  // namespace game