#include "engine/RigidBody.hpp"

#include <cmath>

#include "math/Math.hpp"

namespace engine
{

RigidBody::RigidBody(uint32_t id, float x, float y,  float radius, float mass, float ground_drag_cooef,
    engine::RigidBody::Type type)
: velocity_{}
, rotation_(0.f)
, angular_velocity_(0.f)
, id_(id)
, x_(x)
, y_(y)
, radius_(radius)
, mass_(mass)
, ground_drag_cooef_(ground_drag_cooef)
, type_(type)
{}

void RigidBody::applyForce(sf::Vector2f force)
{
    velocity_ += force;
}

void RigidBody::update(float timeStep)
{
    // Calculate current position based on calculated velocities
    velocity_ *= ground_drag_cooef_;
    angular_velocity_ *= ground_drag_cooef_;
    
    if (std::fabs(angular_velocity_) < 0.1f)
    {
        angular_velocity_ = 0.0f; 
    }

    rotation_ += angular_velocity_ * timeStep;
    rotation_ = std::fmod(rotation_, 360.f);

    x_ += velocity_.x * timeStep;
    y_ += velocity_.y * timeStep;   
}


}  // namespace engine