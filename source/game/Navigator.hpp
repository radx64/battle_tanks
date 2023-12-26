#ifndef GAME_NAVIGATOR_HPP_
#define GAME_NAVIGATOR_HPP_

#include <cmath>

#include "game/Tank.hpp"
#include "math/Math.hpp"

namespace game 
{

//This is a simple class to test driving abilities of tanks.
//I'll rewrite it to LUA whenever I'll start combining project with some lua interpreter.
class Navigator
{
public:
    Navigator(Tank& tank, std::vector<sf::Vector2i>& waypoints);
    void navigate();

protected:
    size_t waypoint_id_{0};
    std::vector<sf::Vector2i>& waypoints_;
    Tank& tank_;
};

}  // namespace game

#endif  // GAME_NAVIGATOR_HPP_