#ifndef GAME_CANNON_HPP_
#define GAME_CANNON_HPP_

#include <SFML/Graphics.hpp>

namespace game 
{

class Cannon
{
public:
    Cannon(double x, double y, double rotation, sf::Texture& texture);
    void draw(sf::RenderWindow& renderWindow);
    void physics(double timeStep);
    void setRotation(double rotation);

    double x_;
    double y_;
    double current_rotation_;
    double set_rotation_;
    sf::Sprite sprite_;
};

}  // namespace game

#endif  // GAME_CANNON_HPP_
