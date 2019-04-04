#ifndef TANK_HPP_
#define TANK_HPP_

#include "IRenderable.hpp"
#include <SFML/Graphics.hpp>

#include "Cannon.hpp"

class Tank : public IRenderable
{
public:
    Tank(float x, float y, float rotation) : x_(x), y_(y), rotation_(rotation), cannon_(x, y, rotation_)
    {
        if (!texture_.loadFromFile("../sprites/tankBody_blue_outline.png"))
            throw std::string("Couldn't load texture file...");

        sprite_.setTexture(texture_);
        sprite_.setPosition(sf::Vector2f(x_, y_));
        sf::Vector2u texture_body_size = texture_.getSize();
        sf::Vector2f texture_body_middle_point(texture_body_size.x / 2.f, texture_body_size.y / 2.f);
        sprite_.setOrigin(texture_body_middle_point);   
    }

    void draw(sf::RenderWindow& renderWindow) override
    {
        sprite_.setColor(sf::Color(10, 10, 10, 127));
        sprite_.setPosition(x_ + 8, y_+ 8);
        sprite_.setRotation(rotation_);
        renderWindow.draw(sprite_);

        sprite_.setColor(sf::Color(255, 255, 255, 255));
        sprite_.setPosition(x_, y_);
        renderWindow.draw(sprite_);
        cannon_.x_ = x_;
        cannon_.y_ = y_;
        cannon_.draw(renderWindow);
    }

public:
    float x_;
    float y_;
    float rotation_;
    sf::Texture texture_;
    sf::Sprite sprite_;
    Cannon cannon_;
};


#endif