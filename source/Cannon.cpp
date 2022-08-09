#include "Cannon.hpp"

#include <cmath>

#include "TextureLibrary.hpp"

// #include <iostream>
// #include <iomanip>


constexpr double CANNON_ROTATION_SPEED = 15.0;

// TODO: make some math header for such things
namespace 
{
double signed_fmod(double a, double n)
{
    return a - floor(a/n) * n;
}
}  // namespace

Cannon::Cannon(uint32_t id, double x, double y, double rotation) : x_(x), y_(y), current_rotation_(rotation)
{
    auto& texture = [](const uint32_t id) -> sf::Texture&
    {
        switch (id % 5)
        {
            case 0 : return TextureLibrary::get("blue_cannon");
            case 1 : return TextureLibrary::get("red_cannon");
            case 2 : return TextureLibrary::get("green_cannon");
            case 3 : return TextureLibrary::get("sand_cannon");
            case 4 : return TextureLibrary::get("dark_cannon");
            default : return TextureLibrary::get("blue_cannon");
        }
    }(id);


    sprite_.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    sf::Vector2f texture_middle_point(texture_size.x/ 2.f, texture_size.y / 2.f + 10.f);
    sprite_.setOrigin(texture_middle_point);
}

void Cannon::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setColor(sf::Color(10, 10, 10, 127));
    sprite_.setPosition(x_ + 3, y_+ 3);
    sprite_.setRotation(current_rotation_ + 90.0);
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
    double delta = set_rotation_ - current_rotation_;
    delta = signed_fmod((delta + 180.0), 360.0) - 180.0;

    if (delta > 0.0) current_rotation_+= std::min(CANNON_ROTATION_SPEED, fabs(delta));
    if (delta < 0.0) current_rotation_-= std::min(CANNON_ROTATION_SPEED, fabs(delta));   
}
