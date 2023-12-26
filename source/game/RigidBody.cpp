#include "RigidBody.hpp"

#include "math/Math.hpp"

namespace game
{

constexpr float SPRING_COLLISION_COEEF = 1.75;

RigidBody::RigidBody(uint32_t id, double x, double y,  double radius, double mass, float ground_drag_cooef)
: id_(id)
, x_(x)
, y_(y)
, radius_(radius)
, mass_(mass)
, ground_drag_cooef_(ground_drag_cooef)
{}

void RigidBody::physics(std::vector<std::unique_ptr<RigidBody>>& objects, double timeStep)
{
    // Process custom physics behaviour first
    onPhysics(objects, timeStep);

    // Then cover general simple circle collsion detection
    for (auto& other_object : objects)
    {
        if (other_object->id_ == id_) continue; // do not check collision with itself.
        
        double distance_between_objects = math::distance(x_, y_, other_object->x_, other_object->y_);
        double sum_of_radius = radius_ + other_object->radius_;

        if (distance_between_objects <= sum_of_radius)
        { 
            // Collision detected here
            // Calculating hit vectors
            double nx = (other_object->x_ - x_) / distance_between_objects;
            double ny = (other_object->y_ - y_) / distance_between_objects;
            double p = SPRING_COLLISION_COEEF * (velocity_.x * nx + velocity_.y * ny
                - other_object->velocity_.x * nx - other_object->velocity_.y * ny) / (mass_ + other_object->mass_);

            sf::Vector2f objectCollisionVelocity = sf::Vector2f(
                (float)(velocity_.x - p * other_object->mass_ * nx),
                (float)(velocity_.y - p * other_object->mass_ * ny));
            
            sf::Vector2f otherObjectCollisionVelocity = sf::Vector2f(
                (float)(other_object->velocity_.x + p * mass_ * nx),
                (float)(other_object->velocity_.y + p * mass_ * ny));

            velocity_ = objectCollisionVelocity;
            other_object->velocity_ = otherObjectCollisionVelocity;

            // Solve static collision to not have on one object on top of each other.
            double objects_overlap = 1.0f  * (distance_between_objects - radius_ - other_object->radius_);
            x_ -= objects_overlap * (x_ - other_object->x_) / distance_between_objects;
            y_ -= objects_overlap * (y_ - other_object->y_) / distance_between_objects;
        }
    };


    // Calculate current position based on calculated velocities
    velocity_ *= ground_drag_cooef_;

    x_ += velocity_.x * timeStep;
    y_ += velocity_.y * timeStep;
}

void RigidBody::onPhysics(std::vector<std::unique_ptr<RigidBody>>& objects, double timeStep)
{
    (void) objects;
    (void) timeStep;
}

}  // namespace gam