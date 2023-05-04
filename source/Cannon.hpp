#ifndef CANNON_HPP_
#define CANNON_HPP_

#include <SFML/Graphics.hpp>

class Cannon
{
public:
    Cannon(double x, double y, double rotation, sf::Texture& texture);
    void draw(sf::RenderWindow& renderWindow);
    void physics(double timeStep);
    void set_rotation(double rotation);

    double x_;
    double y_;
    double current_rotation_;
    double set_rotation_;
    sf::Sprite sprite_;
};


#endif
