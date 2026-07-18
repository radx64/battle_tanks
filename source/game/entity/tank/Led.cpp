#include "game/entity/tank/Led.hpp"

#include <cmath>

#include "engine/math/Math.hpp"

namespace game::entity 
{

Led::Led(float x, float y, sf::Texture& texture)
: x_{x},
y_{y},
timeToChangeState_{rate_}
{
    sprite_.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    sf::Vector2f texture_middle_point = sf::Vector2f(texture_size) / 2.f;
    sprite_.setOrigin(texture_middle_point);
    sprite_.setColor(color_);
}

void Led::setRotation(float rotation)
{
    rotation_ = rotation;
}

void Led::draw(sf::RenderWindow& render_window)
{
    sf::Vector2f rotatedOffset = engine::math::rotatePoint(offset_, rotation_, sf::Vector2f(0.0f, 0.0f));
    sf::Vector2f rotatedPosition = rotatedOffset + sf::Vector2f(x_, y_);
    
    sprite_.setPosition(rotatedPosition);
    if (isOn_)
    {
        sprite_.setColor(color_);
    }
    else
    {
        sprite_.setColor(sf::Color(10, 10, 10, 127));
    }
   render_window.draw(sprite_);
}

void Led::update(float time_step)
{
    if (rate_ <= 0.01) return; // No blinking if rate is 0 or negative

    timeToChangeState_ -= time_step;
    if (timeToChangeState_ <= 0)
    {
        isOn_ = !isOn_;
        timeToChangeState_ = rate_;
    }
}

void Led::setColor(const sf::Color& color)
{
    color_ = color;
}

void Led::setBlinkingRate(float rate)
{
    rate_ = rate;
}

void Led::set(bool state)
{
    isOn_ = state;
}

}  // namespace game::entity
