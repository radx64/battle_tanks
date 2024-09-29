#include "game/Navigator.hpp"

#include <cmath>

#include "game/entity/tank/Tank.hpp"
#include "engine/math/Math.hpp"

namespace game 
{

//How close to the waypoint to treat as visted
constexpr float WAYPOINT_VISITED_DISTANCE = 30.0f; 

Navigator::Navigator(entity::Tank& tank, std::vector<sf::Vector2i>& waypoints) 
: waypoints_(waypoints), tank_(tank)
{}

void Navigator::navigate()
{
    if(waypoints_.empty())
    {
        tank_.setThrottle(0.0);
        return;
    }
    if(waypointId_ >= waypoints_.size()) waypointId_ = 0;
    
    const auto& currentWaypoint = waypoints_[waypointId_];

    auto tankX = tank_.getRigidBody().x_;
    auto tankY = tank_.getRigidBody().y_;   

    float distance = engine::math::distance(currentWaypoint.x, currentWaypoint.y, tankX, tankY);

    if (distance > WAYPOINT_VISITED_DISTANCE)
    {
        float direction = atan2((float)currentWaypoint.y - tankY, (float)currentWaypoint.x - tankX);
        tank_.setThrottle(std::min(1.0, distance*0.01));
        tank_.setDirection(direction * 180.0 / M_PI);        
    }
    else
    {
        waypointId_++;
    }
}

}  // namespace game
