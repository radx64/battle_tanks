#ifndef TANK_HPP_
#define TANK_HPP_

#include "IRenderable.hpp"

#include <SFML/Graphics.hpp>

#include "Cannon.hpp"

class Tank : public IRenderable
{
public:
    Tank(float x, float y, float rotation);
    void draw(sf::RenderWindow& renderWindow) override;

public:
    float x_;
    float y_;
    float rotation_;
    sf::Texture texture_;
    sf::Sprite sprite_;
    Cannon cannon_;
};


#endif