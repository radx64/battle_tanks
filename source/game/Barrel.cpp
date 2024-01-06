#include "game/Barrel.hpp"

#include <SFML/Graphics.hpp>

#include "game/InstanceIdGenerator.hpp"

namespace game
{

constexpr float CRATE_RADIUS = 12;
constexpr float CRATE_MASS = 2;
constexpr float GROUND_DRAG_COEEF = 0.93;

Barrel::Barrel(float x, float y, sf::Texture& barrelBody)
: RigidBody(InstanceIdGenerator::getId(), x, y, CRATE_RADIUS, CRATE_MASS, GROUND_DRAG_COEEF, RigidBody::Type::DYNAMIC)
, renderer_(this, barrelBody)
{ }

void Barrel::draw(sf::RenderWindow& renderWindow)
{
    renderer_.draw(renderWindow);
}

}  // namespace game