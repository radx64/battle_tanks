#include "game/Barrel.hpp"

#include <SFML/Graphics.hpp>

#include "game/InstanceIdGenerator.hpp"
#include "game/BarrelRenderer.hpp"
#include "game/RigidBody.hpp"

namespace game
{

constexpr float CRATE_RADIUS = 12;
constexpr float CRATE_MASS = 2;
constexpr float GROUND_DRAG_COEEF = 0.93;

Barrel::Barrel(float x, float y, sf::Texture& barrelBody)
{ 
    renderer_ = std::make_unique<BarrelRenderer>(this, barrelBody);
    rigid_body_ = std::make_unique<RigidBody>(
        InstanceIdGenerator::getId(), 
        x, y, CRATE_RADIUS, 
        CRATE_MASS, 
        GROUND_DRAG_COEEF, 
        RigidBody::Type::DYNAMIC);

}

}  // namespace game