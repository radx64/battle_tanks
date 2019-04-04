#include "Cannon.hpp"

Cannon::Cannon(float x, float y, float rotation) : x_(x), y_(y), rotation_(rotation)
{
    if (!texture_.loadFromFile("../sprites/tankBlue_barrel1_outline.png"))
        throw std::string("Couldn't load texture file...");

    sprite_.setTexture(texture_);
    sprite_.setPosition(sf::Vector2f(x_, y_));
    sf::Vector2u texture_size = texture_.getSize();
    sf::Vector2f texture_middle_point(texture_size.x/ 2.f, texture_size.y / 2.f + 10.f);
    sprite_.setOrigin(texture_middle_point);
}

void Cannon::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setColor(sf::Color(10, 10, 10, 127));
    sprite_.setPosition(x_ + 3, y_+ 3);
    sprite_.setRotation(rotation_);
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255));
    sprite_.setPosition(x_, y_);
    sprite_.setRotation(rotation_);
    renderWindow.draw(sprite_);
}