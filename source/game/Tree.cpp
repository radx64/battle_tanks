#include "game/Tree.hpp"

#include <SFML/Graphics.hpp>

#include "graphics/TextureLibrary.hpp"

#include "game/InstanceIdGenerator.hpp"

namespace game
{

constexpr float GROUND_DRAG_COEEF = 0.1f;
constexpr float TREE_MASS = 999999.f;

Tree::Tree(float x, float y, sf::Texture& treeBody, float treeTrunkRadius)
: RigidBody(InstanceIdGenerator::getId(), x, y, treeTrunkRadius, TREE_MASS, GROUND_DRAG_COEEF)
{

    sprite_.setTexture(treeBody);
    sf::Vector2u texture_body_size = treeBody.getSize();
    auto middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    sprite_.setOrigin(middle_point);
}

void Tree::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setRotation(angle_);
    sprite_.setColor(sf::Color(10, 10, 10, 127));
    sprite_.setPosition(x_ + 16, y_+ 16);
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255));
    sprite_.setPosition(x_, y_);
    renderWindow.draw(sprite_);
}

}  // namespace game