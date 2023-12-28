#include "RigidBody.hpp"

#include <cmath>

#include "math/Math.hpp"
namespace game
{

constexpr float SPRING_COLLISION_COEEF = 1.75;  // simulate bouciness of collision

constexpr float TANGENT_VELOCITY_SCALING_FACTOR = 25.f;

RigidBody::RigidBody(uint32_t id, float x, float y,  float radius, float mass, float ground_drag_cooef)
: velocity_{}
, angle_(0.f)
, angular_velocity_(0.f)
, id_(id)
, x_(x)
, y_(y)
, radius_(radius)
, mass_(mass)
, ground_drag_cooef_(ground_drag_cooef)
{}

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

        if (distance_between_objects <= sum_of_radius)
        { 
            // Collision detected here
            // Calculating hit vectors
            float nx = (other_object->x_ - x_) / distance_between_objects;
            float ny = (other_object->y_ - y_) / distance_between_objects;
            float p = SPRING_COLLISION_COEEF * (velocity_.x * nx + velocity_.y * ny
                - other_object->velocity_.x * nx - other_object->velocity_.y * ny) / (mass_ + other_object->mass_);

            // Additional angular velocity based on the point of collision
            // So objects rotate when hit
            float tangentVelocity = nx * distance_between_objects * std::sin(math::degree_to_radians(angle_)) - ny * distance_between_objects * std::cos(math::degree_to_radians(angle_));
            angular_velocity_ += TANGENT_VELOCITY_SCALING_FACTOR * tangentVelocity / radius_;
            other_object->angular_velocity_ -= tangentVelocity / other_object->radius_;

            sf::Vector2f objectCollisionVelocity = sf::Vector2f(
                (velocity_.x - p * other_object->mass_ * nx),
                (velocity_.y - p * other_object->mass_ * ny));
            
            sf::Vector2f otherObjectCollisionVelocity = sf::Vector2f(
                (other_object->velocity_.x + p * mass_ * nx),
                (other_object->velocity_.y + p * mass_ * ny));

            velocity_ = objectCollisionVelocity;
            other_object->velocity_ = otherObjectCollisionVelocity;

            // Solve static collision to not have on one object on top of each other.
            float objects_overlap = 1.0 * (distance_between_objects - radius_ - other_object->radius_);
            x_ -= objects_overlap * (x_ - other_object->x_) / distance_between_objects;
            y_ -= objects_overlap * (y_ - other_object->y_) / distance_between_objects;
        }
    };

    // Calculate current position based on calculated velocities
    velocity_ *= ground_drag_cooef_;
    angular_velocity_ *= ground_drag_cooef_;
    
    if (std::fabs(angular_velocity_) < 0.01f)
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