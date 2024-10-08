#include "game/entity/barrel/Barrel.hpp"

#include <SFML/Graphics.hpp>

#include "game/entity/barrel/BarrelRenderer.hpp"
#include "game/InstanceIdGenerator.hpp"
#include "engine/RigidBody.hpp"

namespace game::entity
{

constexpr float BARREL_RADIUS = 12;
constexpr float BARREL_MASS = 2;
constexpr float GROUND_DRAG_COEEF = 0.93;

Barrel::Barrel(float x, float y, sf::Texture& barrelBody)
{ 
    renderer_ = std::make_unique<BarrelRenderer>(this, barrelBody);
    rigidBody_ = std::make_unique<engine::RigidBody>(
        InstanceIdGenerator::getId(), 
        x, y, BARREL_RADIUS, 
        BARREL_MASS, 
        GROUND_DRAG_COEEF, 
        engine::RigidBody::Type::DYNAMIC);

}

}  // namespace game::entity
