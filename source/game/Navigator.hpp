#pragma once


#include <cmath>
#include <vector>

#include <SFML/System.hpp>

namespace game::entity {class Tank;}

namespace game 
{

//This is a simple class to test driving abilities of tanks.
//I'll rewrite it to LUA whenever I'll start combining project with some lua interpreter.
class Navigator
{
public:
    Navigator(entity::Tank& tank, std::vector<sf::Vector2i>& waypoints);
    void navigate();

protected:
    size_t waypoint_id_{0};
    std::vector<sf::Vector2i>& waypoints_;
    entity::Tank& tank_;
};

}  // namespace game

