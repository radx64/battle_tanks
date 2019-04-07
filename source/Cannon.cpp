#include "Cannon.hpp"

#include <cmath>

#include "TextureLibrary.hpp"

constexpr float TANK_ROTATION_SPEED = 6.0f;

Cannon::Cannon(float x, float y, float rotation) : x_(x), y_(y), rotation_(rotation)
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
    sprite_.setRotation(rotation_ + 90.f);
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255));
    sprite_.setPosition(x_, y_);
    renderWindow.draw(sprite_);
}

void Cannon::set_rotation(float rotation)
{
    set_rotation_ = rotation;
}

void Cannon::physics()
{
    if (rotation_ > 360.f) rotation_ = 0.f;
    if (rotation_ < 0.f) rotation_ = 359.f;

    float delta = set_rotation_ - rotation_;
    while (delta < 0.f) delta +=360.f;

    if (delta < 180.f) rotation_+= std::min(TANK_ROTATION_SPEED, (float) fabs(delta));
    if (delta > 180.f) rotation_-= std::min(TANK_ROTATION_SPEED, (float) fabs(delta));   
}