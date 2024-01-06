#ifndef GAME_TANK_HPP_
#define GAME_TANK_HPP_

#include <memory>

#include <SFML/Graphics.hpp>

#include "game/Cannon.hpp"
#include "game/Renderer.hpp"
#include "game/RigidBody.hpp"
#include "game/TankRenderer.hpp"

namespace game 
{

class Tank : public Renderer, public RigidBody
{
public:
    Tank(float x, float y, float rotation, 
        std::unique_ptr<Cannon> cannon, sf::Texture& tankBody);
    virtual ~Tank() = default;

    void draw(sf::RenderWindow& renderWindow) override;

    void setThrottle(float throttle);
    void setDirection(float direction);
    static void setDebug(bool is_enabled); 

    float current_direction_{};
    sf::Vector2f drivetrain_force_{};
    sf::Vector2f braking_force_{};
    sf::Sprite sprite_;
    std::unique_ptr<Cannon> cannon_;

protected:
    void drawDebugInfo(sf::RenderWindow& renderWindow);

    void onPhysics(std::vector<std::unique_ptr<RigidBody>>& objects, float timeStep) override;

    sf::Vector2f tank_middle_point_;

    float set_direction_{};
    float set_throttle_{};
    float current_throttle_{};
    static bool DEBUG_;
    TankRenderer renderer_;
};

}  // namespace game

#endif  // GAME_TANK_HPP_
