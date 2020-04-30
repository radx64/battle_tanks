#ifndef TANK_HPP_
#define TANK_HPP_

#include "IRenderable.hpp"

#include <SFML/Graphics.hpp>

#include "Cannon.hpp"

class Tank : public IRenderable
{
public:
    Tank(double x, double y, double rotation);
    void draw(sf::RenderWindow& renderWindow) override;
    void physics();

    void set_throtle(double throttle);
    void set_direction(double direction);

public:
    double x_{};
    double y_{};
    double direction_{};
    double set_direction_{};
    double set_throttle_{};
    double velocity_{};
    sf::Sprite sprite_;
    sf::Text text_;
    Cannon cannon_;
};


#endif
