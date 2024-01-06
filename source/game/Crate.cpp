#include "game/Crate.hpp"

#include <SFML/Graphics.hpp>

#include "game/InstanceIdGenerator.hpp"
#include "game/CrateRenderer.hpp"
#include "game/RigidBody.hpp"

namespace game
{

constexpr float CRATE_RADIUS = 16;
constexpr float CRATE_MASS = 1;
constexpr float GROUND_DRAG_COEEF = 0.85;


Crate::Crate(float x, float y, sf::Texture& crateBody)
{ 
    renderer_ = std::make_unique<CrateRenderer>(this, crateBody);
    rigid_body_ = std::make_unique<RigidBody>(
        InstanceIdGenerator::getId(), 
        x, y, CRATE_RADIUS, 
        CRATE_MASS, 
        GROUND_DRAG_COEEF, 
        RigidBody::Type::DYNAMIC);
}

}  // namespace game