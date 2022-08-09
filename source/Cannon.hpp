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
    Cannon(uint32_t id, double x, double y, double rotation);
    void draw(sf::RenderWindow& renderWindow);
    void physics();
    void set_rotation(double rotation);

    double x_;
    double y_;
    double current_rotation_;
    double set_rotation_;
    sf::Sprite sprite_;
};


#endif
