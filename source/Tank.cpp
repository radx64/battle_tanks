#include "Tank.hpp"

#include <algorithm>
#include <cmath>

#include "TextureLibrary.hpp"
#include "FontLibrary.hpp"

constexpr float TANK_MAX_VELOCITY = 2.0f;
constexpr float TANK_BRAKE_FORCE = 0.9f;
constexpr float TANK_ACCELERATION = 0.02f;
constexpr float TANK_ROTATION_SPEED = 2.5f;

Tank::Tank(float x, float y, float rotation) : x_(x), y_(y), direction_(rotation), set_direction_(rotation), cannon_(x, y, direction_)
{
    auto& texture = TextureLibrary::get("blue_tank");
    sprite_.setTexture(texture);
    sprite_.setPosition(sf::Vector2f(x_, y_));
    sf::Vector2u texture_body_size = texture.getSize();
    sf::Vector2f texture_body_middle_point(texture_body_size.x / 2.f, texture_body_size.y / 2.f);
    sprite_.setOrigin(texture_body_middle_point);   
    text_.setFont(FontLibrary::get("armata"));
}

void Tank::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setColor(sf::Color(10, 10, 10, 127));
    sprite_.setPosition(x_ + 8, y_+ 8);
    sprite_.setRotation(direction_ - 90.f);
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255));
    sprite_.setPosition(x_, y_);
    renderWindow.draw(sprite_);
    cannon_.x_ = x_;
    cannon_.y_ = y_;
    cannon_.draw(renderWindow);

    // text_.setString("X: " + std::to_string(x_) 
    //     + "\nY:" + std::to_string(y_)
    //     + "\nD:" + std::to_string(direction_));
    // text_.setPosition(x_ + 20.f, y_ - 40.f);
    // text_.setCharacterSize(10);
    // renderWindow.draw(text_);
}

void Tank::set_throtle(float throttle)
{
    set_throttle_ = throttle;
}
void Tank::set_direction(float direction)
{
    set_direction_ = direction;
    cannon_.set_rotation(direction);
}

void Tank::physics()
{
    if(velocity_<= TANK_MAX_VELOCITY * set_throttle_) velocity_ += set_throttle_ * TANK_ACCELERATION;
    else velocity_ *= TANK_BRAKE_FORCE;
    if (direction_ > 360.f) direction_ = 0.f;
    if (direction_ < 0.f) direction_ = 359.f;

    float delta = set_direction_ - direction_;
    while (delta < 0.f) delta +=360.f;

    if (delta < 180.f) direction_+= std::min(TANK_ROTATION_SPEED, (float) fabs(delta));
    if (delta > 180.f) direction_-= std::min(TANK_ROTATION_SPEED, (float) fabs(delta));

    x_ += cos(direction_ * M_PI/180.f) * velocity_;
    y_ += sin(direction_ * M_PI/180.f) * velocity_;
    cannon_.physics();
}