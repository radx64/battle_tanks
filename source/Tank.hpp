#ifndef TANK_HPP_
#define TANK_HPP_

#include "IRenderable.hpp"

#include <SFML/Graphics.hpp>

#include "Cannon.hpp"

class Tank : public IRenderable
{
public:
    Tank(uint32_t id, double x, double y, double rotation);
    void draw(sf::RenderWindow& renderWindow) override;
    void physics(std::vector<Tank*>& tanks);

    void set_throtle(double throttle);
    void set_direction(double direction);

public:
    uint32_t id_;
    double x_{};
    double y_{};
    double current_direction_{};
    double set_direction_{};
    double set_throttle_{};
    double current_throttle_{};
    sf::Vector2f drivetrain_force_{};
    sf::Vector2f braking_force_{};
    sf::Sprite sprite_;
    sf::Text text_;
    Cannon cannon_;

protected:
    sf::Vector2f tank_middle_point_;
    sf::Vector2f velocity_;

};


#endif
