#include "Cannon.hpp"

#include <cmath>

#include "TextureLibrary.hpp"

constexpr double TANK_ROTATION_SPEED = 8.0;

Cannon::Cannon(double x, double y, double rotation) : x_(x), y_(y), rotation_(rotation)
{
    auto& texture = TextureLibrary::get("blue_cannon");
    sprite_.setTexture(texture);
    sprite_.setPosition(sf::Vector2f(x_, y_));
    sf::Vector2u texture_size = texture.getSize();
    sf::Vector2f texture_middle_point(texture_size.x/ 2.f, texture_size.y / 2.f + 10.f);
    sprite_.setOrigin(texture_middle_point);
}

void Cannon::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setColor(sf::Color(10, 10, 10, 127));
    sprite_.setPosition(x_ + 3, y_+ 3);
    sprite_.setRotation(rotation_ + 90.0);
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255));
    sprite_.setPosition(x_, y_);
    renderWindow.draw(sprite_);
}

void Cannon::set_rotation(double rotation)
{
    set_rotation_ = rotation;
}

void Cannon::physics()
{
    if (rotation_ > 360.0) rotation_ = 0.0;
    if (rotation_ < 0.0) rotation_ = 359.0;

    double delta = set_rotation_ - rotation_;
    while (delta < 0.0) delta += 360.0;

    if (delta <= 180.0) rotation_+= std::min(TANK_ROTATION_SPEED, (double) fabs(delta));
    if (delta > 180.0) rotation_-= std::min(TANK_ROTATION_SPEED, (double) fabs(delta));   
}
