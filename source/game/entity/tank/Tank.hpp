#ifndef GAME_ENTITY_TANK_TANK_HPP_
#define GAME_ENTITY_TANK_TANK_HPP_

#include <memory>

#include <SFML/Graphics.hpp>

#include "game/entity/tank/Cannon.hpp"
#include "engine/GameObject.hpp"

namespace game::entity {class TracksRenderer;}
namespace game::entity 
{

class Tank : public engine::GameObject
{
public:
    Tank(float x, float y, float rotation, 
        std::unique_ptr<Cannon> cannon, 
        sf::Texture& tankBody,
        TracksRenderer* tracksRenderer);
    virtual ~Tank() = default;

    void onUpdate(engine::Scene& scene, float timeStep) override;
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
    TracksRenderer* tracks_renderer_;
};

}  // namespace game::entity

#endif  // GAME_ENTITY_TANK_TANK_HPP_
