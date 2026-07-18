#include "game/entity/crate/Crate.hpp"

#include <SFML/Graphics.hpp>

#include "game/InstanceIdGenerator.hpp"
#include "game/entity/crate/CrateRenderer.hpp"
#include "engine/RigidBody.hpp"

namespace game::entity
{

constexpr float CRATE_RADIUS = 16;
constexpr float CRATE_MASS = 1;
constexpr float GROUND_DRAG_COEEF = 0.85;

Crate::Crate(float x, float y, sf::Texture& crateBody)
{ 
    transform().position().x = x;
    transform().position().y = y;

    renderer_ = std::make_unique<CrateRenderer>(this, crateBody);
    rigid_body_ = std::make_unique<engine::RigidBody>(
        InstanceIdGenerator::getId(), 
        transform(), CRATE_RADIUS, 
        CRATE_MASS, 
        GROUND_DRAG_COEEF, 
        engine::RigidBody::Type::DYNAMIC);
}

}  // namespace game::entity
