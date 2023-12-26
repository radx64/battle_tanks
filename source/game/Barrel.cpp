#include "game/Barrel.hpp"

#include <SFML/Graphics.hpp>

namespace game
{

constexpr double BARREL_RADIUS = 12;
constexpr double BARREL_MASS = 2;

constexpr float GROUND_DRAG_COEEF = 0.93;


Barrel::Barrel(uint32_t id, double x, double y, sf::Texture& barrelBody)
: RigidBody(id, x, y, BARREL_RADIUS, BARREL_MASS, GROUND_DRAG_COEEF)
{
    sprite_.setTexture(barrelBody);
    sf::Vector2u texture_body_size = barrelBody.getSize();
    auto middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    sprite_.setOrigin(middle_point);
}

void Barrel::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setColor(sf::Color(10, 10, 10, 127));
    sprite_.setPosition(x_ + 4, y_+ 4);
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255));
    sprite_.setPosition(x_, y_);
    renderWindow.draw(sprite_);
}


}  // namespace game