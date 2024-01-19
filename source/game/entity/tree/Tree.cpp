#include "game/entity/tree/Tree.hpp"

#include <SFML/Graphics.hpp>

#include "graphics/TextureLibrary.hpp"

#include "game/entity/tree/TreeRenderer.hpp"
#include "game/InstanceIdGenerator.hpp"
#include "engine/RigidBody.hpp"

namespace game::entity
{

constexpr float GROUND_DRAG_COEEF = 0.1f;
constexpr float TREE_MASS = 500.f;

Tree::Tree(float x, float y, float height, sf::Texture& treeBody, float treeTrunkRadius)
: height_(height)
{ 
    renderer_ = std::make_unique<TreeRenderer>(this, treeBody);

    rigid_body_ = std::make_unique<engine::RigidBody>(
        InstanceIdGenerator::getId(), 
        x, y, treeTrunkRadius, 
        TREE_MASS, 
        GROUND_DRAG_COEEF, 
        engine::RigidBody::Type::STATIC);
}

}  // namespace game::entity