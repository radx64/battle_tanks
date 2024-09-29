#include "engine/RigidBody.hpp"

#include <cmath>

#include "engine/math/Math.hpp"

namespace engine
{

RigidBody::RigidBody(uint32_t id, float x, float y,  float radius, float mass, float groundDragCooef,
    engine::RigidBody::Type type)
: velocity_{}
, rotation_(0.f)
, angularVelocity_(0.f)
, id_(id)
, x_(x)
, y_(y)
, radius_(radius)
, mass_(mass)
, groundDragCooef_(groundDragCooef)
, type_(type)
{}

void RigidBody::applyForce(sf::Vector2f force)
{
    velocity_ += force;
}

void RigidBody::update(float timeStep)
{
    // Calculate current position based on calculated velocities
    velocity_ *= groundDragCooef_;
    angularVelocity_ *= groundDragCooef_;
    
    if (std::fabs(angularVelocity_) < 0.1f)
    {
        angularVelocity_ = 0.0f; 
    }

    rotation_ += angularVelocity_ * timeStep;
    rotation_ = std::fmod(rotation_, 360.f);

    x_ += velocity_.x * timeStep;
    y_ += velocity_.y * timeStep;   
}


}  // namespace engine
