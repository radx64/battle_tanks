#ifndef CANNON_HPP_
#define CANNON_HPP_

#include <SFML/Graphics.hpp>

namespace sf
{
    class RenderWindow;
}

class Cannon
{
public:
    Cannon(double x, double y, double rotation);
    void draw(sf::RenderWindow& renderWindow);
    void physics();
    void set_rotation(double rotation);

    double x_;
    double y_;
    double rotation_;
    double set_rotation_;
    sf::Sprite sprite_;
};


#endif
