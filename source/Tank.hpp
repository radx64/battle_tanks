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
    void physics();

    void set_throtle(float throttle);
    void set_direction(float direction);

public:
    float x_{};
    float y_{};
    float direction_{};
    float set_direction_{};
    float set_throttle_{};
    float velocity_{};
    sf::Texture texture_;
    sf::Font font_;
    sf::Sprite sprite_;
    sf::Text text_;
    Cannon cannon_;
};


#endif