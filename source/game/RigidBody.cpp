#include "RigidBody.hpp"

#include <cmath>

#include "math/Math.hpp"

#include <iostream>

namespace game
{

constexpr float SPRING_COLLISION_COEEF = 1.75;  // simulate bouciness of collision

constexpr float TANGENT_VELOCITY_SCALING_FACTOR = 500.f;

RigidBody::RigidBody(uint32_t id, float x, float y,  float radius, float mass, float ground_drag_cooef,
    RigidBody::Type type)
: velocity_{}
, angle_(0.f)
, angular_velocity_(0.f)
, id_(id)
, x_(x)
, y_(y)
, radius_(radius)
, mass_(mass)
, ground_drag_cooef_(ground_drag_cooef)
, type_(type)
{}

void RigidBody::processStaticAndDynamicObjectsCollsion(RigidBody* static_object, RigidBody* dynamic_object)
{
    (void) static_object;
    (void) dynamic_object;

}

void RigidBody::processDynamicObjectsCollsion(RigidBody* dynamic_object, RigidBody* other_dynamic_object)
{
    float distance_between_objects = math::distance(dynamic_object->x_, dynamic_object->y_, 
        other_dynamic_object->x_, other_dynamic_object->y_);
    // Collision detected here
    // Calculating hit vectors
    float nx = (other_dynamic_object->x_ - dynamic_object->x_) / distance_between_objects;
    float ny = (other_dynamic_object->y_ - dynamic_object->y_) / distance_between_objects;
    float p = SPRING_COLLISION_COEEF * (
        dynamic_object->velocity_.x * nx +dynamic_object->velocity_.y * ny 
        - other_dynamic_object->velocity_.x * nx - other_dynamic_object->velocity_.y * ny) 
        / (dynamic_object->mass_ + other_dynamic_object->mass_);

    // Additional angular velocity based on the point of collision
    // So objects rotate when hit

    // FIXME: There is something wrong in this tangent velocity formula.
    // In circle collision rotation of a circle should not matter thus angle should
    // not be needed (only relative position of circles, point of contact and their velocities)
    // I need to investigate it further
    // float tangentVelocity = nx * distance_between_objects * std::sin(math::degree_to_radians(dynamic_object->angle_)) 
    //     - ny * distance_between_objects * std::cos(math::degree_to_radians(dynamic_object->angle_));
    // For now I will hardcode this as a constant velocity just for testing purposes

    float tangentVelocity =  TANGENT_VELOCITY_SCALING_FACTOR;

    // This cross product is to determine rotation direction of the angular momentum    
    float crossProduct = 
        dynamic_object->velocity_.x  * other_dynamic_object->velocity_.y 
        - dynamic_object->velocity_.y  * other_dynamic_object->velocity_.x;

    if (crossProduct > 0) tangentVelocity = tangentVelocity * -1.f;

    angular_velocity_ += tangentVelocity / dynamic_object->radius_;
    other_dynamic_object->angular_velocity_ -= tangentVelocity / other_dynamic_object->radius_;

    sf::Vector2f objectCollisionVelocity = sf::Vector2f(
        (dynamic_object->velocity_.x - p * other_dynamic_object->mass_ * nx),
        (dynamic_object->velocity_.y - p * other_dynamic_object->mass_ * ny));
    
    dynamic_object->velocity_ = objectCollisionVelocity;

    sf::Vector2f otherObjectCollisionVelocity = sf::Vector2f(
        (other_dynamic_object->velocity_.x + p * dynamic_object->mass_ * nx),
        (other_dynamic_object->velocity_.y + p * dynamic_object->mass_ * ny));
    other_dynamic_object->velocity_ = otherObjectCollisionVelocity;
}

void RigidBody::physics(std::vector<std::unique_ptr<RigidBody>>& objects, float timeStep)
{
    // Process custom physics behaviour first
    onPhysics(objects, timeStep);

    // Then cover general simple circle collsion detection
    for (auto& other_object : objects)
    {
        if (other_object->id_ == id_) continue; // do not check collision with itself.
        
        float distance_between_objects = math::distance(x_, y_, other_object->x_, other_object->y_);
        float sum_of_radius = radius_ + other_object->radius_;

        if (distance_between_objects >= sum_of_radius)
        {
            continue ;  // no collsion
        }

        if (type_ == RigidBody::Type::STATIC)
        {
            if (other_object->type_ == RigidBody::Type::STATIC)
            {
                // Collision of static objects should not do anything
                // TODO: add onCollision handler
            } 
            else /* other object Type::Dynamic */
            {
                processStaticAndDynamicObjectsCollsion(this, other_object.get());
            }

        } 
        else /* Type::Dynamic */
        {
            if (other_object->type_ == RigidBody::Type::STATIC)
            {
                processStaticAndDynamicObjectsCollsion(other_object.get(), this);
            } 
            else /* other object Type::Dynamic */
            {
                processDynamicObjectsCollsion(this, other_object.get());
            }

            // Solve static collision to not have on one object on top of each other.
            float objects_overlap = 1.0 * (distance_between_objects - radius_ - other_object->radius_);
            x_ -= objects_overlap * (x_ - other_object->x_) / distance_between_objects;
            y_ -= objects_overlap * (y_ - other_object->y_) / distance_between_objects;
        }
    }


    // Calculate current position based on calculated velocities
    velocity_ *= ground_drag_cooef_;
    angular_velocity_ *= ground_drag_cooef_;
    
    if (std::fabs(angular_velocity_) < 0.1f)
    {
        angular_velocity_ = 0.0f; 
    }

    angle_ += angular_velocity_ * timeStep;
    angle_ = std::fmod(angle_, 360.f);

    x_ += velocity_.x * timeStep;
    y_ += velocity_.y * timeStep;
}

void RigidBody::onPhysics(std::vector<std::unique_ptr<RigidBody>>& objects, float timeStep)
{
    (void) objects;
    (void) timeStep;
}

void RigidBody::applyForce(sf::Vector2f force)
{
    velocity_ += force;
}

}  // namespace gam