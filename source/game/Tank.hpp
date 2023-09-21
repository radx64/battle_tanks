#ifndef TANK_HPP_
#define TANK_HPP_

#include "IRenderable.hpp"

#include <memory>

#include <SFML/Graphics.hpp>

#include "game/Cannon.hpp"

class Tank : public IRenderable
{
public:
    Tank(uint32_t id, double x, double y, double rotation, 
        std::unique_ptr<Cannon> cannon, sf::Texture& tankBody);
    void draw(sf::RenderWindow& renderWindow) override;
    void physics(std::vector<std::unique_ptr<Tank>>& tank, double timeStep);

    void set_throtle(double throttle);
    void set_direction(double direction);
    static void set_debug(bool is_enabled); 

public:
    uint32_t id_;
    double x_{};
    double y_{};
    sf::Vector2f drivetrain_force_{};
    sf::Vector2f braking_force_{};
    sf::Sprite sprite_;
    std::unique_ptr<Cannon> cannon_;

protected:
    void drawDebugInfo(sf::RenderWindow& renderWindow);
    sf::Vector2f tank_middle_point_;
    sf::Vector2f velocity_;
    double current_direction_{};
    double set_direction_{};
    double set_throttle_{};
    double current_throttle_{};
    static bool DEBUG_;
};


#endif