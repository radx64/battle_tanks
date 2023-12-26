#include "game/Navigator.hpp"

#include <cmath>

#include "game/Tank.hpp"
#include "math/Math.hpp"

namespace game 
{

//How close to the waypoint to treat as visted
constexpr float WAYPOINT_VISITED_DISTANCE = 30.0f; 

Navigator::Navigator(Tank& tank, std::vector<sf::Vector2i>& waypoints) 
: waypoints_(waypoints), tank_(tank)
{}

void Navigator::navigate()
{
    if(waypoints_.empty())
    {
        tank_.setThrottle(0.0);
        return;
    }
    if(waypoint_id_ >= waypoints_.size()) waypoint_id_ = 0;
    
    const auto& current_waypoint = waypoints_[waypoint_id_];

    double distance = math::distance(current_waypoint.x, current_waypoint.y, tank_.x_, tank_.y_);

    if (distance > WAYPOINT_VISITED_DISTANCE)
    {
        double direction = atan2((double)current_waypoint.y - tank_.y_, (double)current_waypoint.x - tank_.x_);
        tank_.setThrottle(std::min(1.0, distance*0.01));
        tank_.setDirection(direction * 180.0 / M_PI);        
    }
    else
    {
        waypoint_id_++;
    }
}

}  // namespace game