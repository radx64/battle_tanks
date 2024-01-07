#ifndef GAME_TANK_HPP_
#define GAME_TANK_HPP_

#include <memory>

#include <SFML/Graphics.hpp>

#include "game/Cannon.hpp"
#include "game/GameObject.hpp"

namespace game 
{

class Tank : public GameObject
{
public:
    Tank(float x, float y, float rotation, 
        std::unique_ptr<Cannon> cannon, sf::Texture& tankBody);
    virtual ~Tank() = default;

    void onUpdate(game::World& world, float timeStep) override;
    void setThrottle(float throttle);
    void setDirection(float direction);
    static void setDebug(bool is_enabled); 

    float current_direction_{};
    float current_throttle_{};
    sf::Vector2f drivetrain_force_{};
    sf::Vector2f braking_force_{};
    std::unique_ptr<Cannon> cannon_;
    static bool DEBUG_;

protected:
    sf::Vector2f tank_middle_point_;
    float set_direction_{};
    float set_throttle_{};
    float lifetime_{};
};

}  // namespace game

#endif  // GAME_TANK_HPP_
