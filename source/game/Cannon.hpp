#ifndef GAME_CANNON_HPP_
#define GAME_CANNON_HPP_

#include <SFML/Graphics.hpp>

namespace game 
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

}  // namespace game

#endif  // GAME_CANNON_HPP_
