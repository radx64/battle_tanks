#include "Cannon.hpp"

#include "TextureLibrary.hpp"

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