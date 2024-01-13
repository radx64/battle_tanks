#include "game/Tree.hpp"

#include <SFML/Graphics.hpp>

#include "graphics/TextureLibrary.hpp"

#include "game/InstanceIdGenerator.hpp"
#include "game/TreeRenderer.hpp"
#include "game/RigidBody.hpp"

namespace game
{

constexpr float GROUND_DRAG_COEEF = 0.1f;
constexpr float TREE_MASS = 500.f;

Tree::Tree(float x, float y, float height, sf::Texture& treeBody, float treeTrunkRadius)
: height_(height)
{ 
    renderer_ = std::make_unique<TreeRenderer>(this, treeBody);

    rigid_body_ = std::make_unique<RigidBody>(
        InstanceIdGenerator::getId(), 
        x, y, treeTrunkRadius, 
        TREE_MASS, 
        GROUND_DRAG_COEEF, 
        RigidBody::Type::STATIC);
}

}  // namespace game