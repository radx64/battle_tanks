#pragma once


#include <SFML/Graphics.hpp>

namespace game::entity 
{

class Cannon
{
public:
    Cannon(float x, float y, float rotation, sf::Texture& texture);
    void draw(sf::RenderWindow& renderWindow);
    void physics(float timeStep);
    void setRotation(float rotation);
    void fire();

    float x_;
    float y_;
    float current_rotation_;
    float set_rotation_;
    float cooldown_;
    sf::Sprite sprite_;
};

}  // namespace game::entity


