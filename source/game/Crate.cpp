#include "game/Crate.hpp"

#include <SFML/Graphics.hpp>

#include "game/InstanceIdGenerator.hpp"

namespace game
{

constexpr float CRATE_RADIUS = 16;
constexpr float CRATE_MASS = 1;
constexpr float GROUND_DRAG_COEEF = 0.85;


Crate::Crate(float x, float y, sf::Texture& crateBody)
: RigidBody(InstanceIdGenerator::getId(), x, y, CRATE_RADIUS, CRATE_MASS, GROUND_DRAG_COEEF, RigidBody::Type::DYNAMIC),
renderer_(this, crateBody)
{ }

void Crate::draw(sf::RenderWindow& renderWindow)
{
    renderer_.draw(renderWindow);
}

}  // namespace game