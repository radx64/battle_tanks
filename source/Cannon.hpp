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
    Cannon(float x, float y, float rotation);
    void draw(sf::RenderWindow& renderWindow);
    void physics();
    void set_rotation(float rotation);

    float x_;
    float y_;
    float rotation_;
    float set_rotation_;
    sf::Sprite sprite_;
};


#endif