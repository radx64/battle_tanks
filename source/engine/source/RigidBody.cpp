#include "engine/RigidBody.hpp"

#include <cmath>

#include "engine/math/Math.hpp"

namespace engine
{

RigidBody::RigidBody(uint32_t id,  Transform& transform, float radius, float mass, float groundDragCooef,
    engine::RigidBody::Type type)
: velocity_{}
, angularVelocity_(0.f)
, id_(id)
, transform_(transform)
, radius_(radius)
, mass_(mass)
, groundDragCooef_(groundDragCooef)
, type_(type)
{}

void RigidBody::applyForce(sf::Vector2f force)
{
    velocity_ += force;
}

Transform& RigidBody::transform()
{ 
    return transform_;
}

const Transform& RigidBody::transform() const
{
    return transform_; 
}

void RigidBody::update(float time_step)
{
    // Calculate current position based on calculated velocities
    velocity_ *= groundDragCooef_;
    angularVelocity_ *= groundDragCooef_;
    
    if (std::fabs(angularVelocity_) < 0.1f)
    {
        angularVelocity_ = 0.0f; 
    }

    transform_.rotation() += angularVelocity_ * time_step;
    transform_.rotation() = std::fmod(transform_.rotation(), 360.f);

    transform_.position().x += velocity_.x * time_step;
    transform_.position().y += velocity_.y * time_step;   
}


}  // namespace engine
