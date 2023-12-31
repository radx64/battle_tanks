#include "game/Crate.hpp"

#include <SFML/Graphics.hpp>

#include "game/InstanceIdGenerator.hpp"

namespace game
{

constexpr float CRATE_RADIUS = 16;
constexpr float CRATE_MASS = 1;
constexpr float GROUND_DRAG_COEEF = 0.85;


Crate::Crate(float x, float y, sf::Texture& barrelBody)
: RigidBody(InstanceIdGenerator::getId(), x, y, CRATE_RADIUS, CRATE_MASS, GROUND_DRAG_COEEF, RigidBody::Type::DYNAMIC)
{
    sprite_.setTexture(barrelBody);
    sf::Vector2u texture_body_size = barrelBody.getSize();
    auto middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    sprite_.setOrigin(middle_point);
}

void Crate::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setRotation(angle_);
    sprite_.setColor(sf::Color(10, 10, 10, 127));
    sprite_.setPosition(x_ + 4, y_+ 4);
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255));
    sprite_.setPosition(x_, y_);
    renderWindow.draw(sprite_);
}

}  // namespace game