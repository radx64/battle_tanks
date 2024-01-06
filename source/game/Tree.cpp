#include "game/Tree.hpp"

#include <SFML/Graphics.hpp>

#include "graphics/TextureLibrary.hpp"

#include "game/InstanceIdGenerator.hpp"

namespace game
{

constexpr float GROUND_DRAG_COEEF = 0.1f;
constexpr float TREE_MASS = 5.f;

Tree::Tree(float x, float y, float height, sf::Texture& treeBody, float treeTrunkRadius)
: RigidBody(InstanceIdGenerator::getId(), x, y, treeTrunkRadius, TREE_MASS, GROUND_DRAG_COEEF, RigidBody::Type::STATIC)
, height_(height)
, renderer_(this, treeBody)
{ }

void Tree::draw(sf::RenderWindow& renderWindow)
{
    renderer_.draw(renderWindow);
}

}  // namespace game