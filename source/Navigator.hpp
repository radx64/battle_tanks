#ifndef NAVIGATOR_HPP_
#define NAVIGATOR_HPP_

#include "Tank.hpp"
#include "Math.hpp"


#include <iostream>

class Tank;

class Navigator
{
public:
    Navigator(Tank& tank, std::vector<sf::Vector2i>& waypoints) : waypoints_(waypoints), tank_(tank)
    {}

    void navigate()
    {
        if(waypoints_.empty())
        {
           tank_.set_throtle(0.0);
           std::cout <<"IDLE" <<  std::endl; 
           return;
        }
        if(waypoint_id_ >= waypoints_.size()) waypoint_id_ = 0;
        
        const auto& current_waypoint = waypoints_[waypoint_id_];

        double distance = math::distance(current_waypoint.x, current_waypoint.y, tank_.x_, tank_.y_);

        if (distance > 30.0f)
        {
            double direction = atan2((double)current_waypoint.y - tank_.y_, (double)current_waypoint.x - tank_.x_);
            tank_.set_throtle(std::min(1.0, distance*0.01));
            tank_.set_direction(direction * 180.0 / M_PI);        
        }
        else
        {
            waypoint_id_++;
        }
    }

protected:
    size_t waypoint_id_{0};
    std::vector<sf::Vector2i>& waypoints_;
    Tank& tank_;
};


#endif