#include "RigidBody.hpp"

#include <cmath>

#include "math/Math.hpp"

namespace game
{

constexpr float SPRING_COLLISION_COEEF = 1.75;  // simulate bouciness of collision

constexpr float TANGENT_VELOCITY_SCALING_FACTOR = 50.f;

struct CollisionResult
{
    float tangentVelocity_;     // velocity added to angular velocity of an object
    float impact_;              // linear collision force
    float nx_;                  // normal hit vector x coordinate
    float ny_;                  // normal hit vector y coordinate
};

CollisionResult processObjectsCollsion(RigidBody* object, RigidBody* other_object)
{
   float distance_between_objects = math::distance(object->x_, object->y_, 
        other_object->x_, other_object->y_);
    // Collision detected here
    // Calculating hit vectors
    float nx = (other_object->x_ - object->x_) / distance_between_objects;
    float ny = (other_object->y_ - object->y_) / distance_between_objects;
    float impact = SPRING_COLLISION_COEEF * (
        math::dot_product(object->velocity_.x, object->velocity_.y, nx, ny)
        - math::dot_product(other_object->velocity_.x, other_object->velocity_.y, nx, ny)) 
        / (object->mass_ + other_object->mass_);

    // Additional angular velocity based on the point of collision
    // So objects rotate when hit
    auto relativeVelocity = object->velocity_ - other_object->velocity_;
    float velocityVectorLength = math::normalize_vector(relativeVelocity);
    float dotProduct = math::dot_product(nx, ny, relativeVelocity.x, relativeVelocity.y);
    // TODO: Consider adding mass and impact momentum to this tangent velocity later
    float tangentVelocity =  velocityVectorLength * dotProduct * TANGENT_VELOCITY_SCALING_FACTOR;

    // This cross product is used to determine rotation direction of the angular momentum    
    float crossProduct = math::cross_product(
        object->velocity_.x, 
        object->velocity_.y,
        other_object->velocity_.x, 
        other_object->velocity_.y);

    if (crossProduct > 0) tangentVelocity = tangentVelocity * -1.f;

    return CollisionResult{
        .tangentVelocity_ = tangentVelocity, 
        .impact_ = impact, 
        .nx_ = nx,
        .ny_ = ny};
}

RigidBody::RigidBody(uint32_t id, float x, float y,  float radius, float mass, float ground_drag_cooef,
    RigidBody::Type type)
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


void RigidBody::processStaticAndDynamicObjectsCollsion(RigidBody* static_object, RigidBody* dynamic_object)
{
    auto collisionResult = processObjectsCollsion(static_object, dynamic_object);

    dynamic_object->angular_velocity_ -= collisionResult.tangentVelocity_  / dynamic_object->radius_;

    sf::Vector2f normalVector{collisionResult.nx_,collisionResult.ny_};
    math::normalize_vector(normalVector);

    float relativeSpeed = math::dot_product(
        dynamic_object->velocity_.x, dynamic_object->velocity_.y,
        normalVector.x, normalVector.y);

    float impulse = 2.0f * relativeSpeed;

    dynamic_object->velocity_ = dynamic_object->velocity_ - normalVector * impulse ;
}

void RigidBody::processDynamicObjectsCollsion(RigidBody* dynamic_object, RigidBody* other_dynamic_object)
{ 
    auto collisionResult = processObjectsCollsion(dynamic_object, other_dynamic_object);

    dynamic_object->angular_velocity_ += collisionResult.tangentVelocity_ / dynamic_object->radius_;
    other_dynamic_object->angular_velocity_ -= collisionResult.tangentVelocity_  / other_dynamic_object->radius_;

    sf::Vector2f objectCollisionVelocity = sf::Vector2f(
        (dynamic_object->velocity_.x - collisionResult.impact_ * other_dynamic_object->mass_ * collisionResult.nx_),
        (dynamic_object->velocity_.y - collisionResult.impact_ * other_dynamic_object->mass_ * collisionResult.ny_));
    
    dynamic_object->velocity_ = objectCollisionVelocity;

    sf::Vector2f otherObjectCollisionVelocity = sf::Vector2f(
        (other_dynamic_object->velocity_.x + collisionResult.impact_ * dynamic_object->mass_ * collisionResult.nx_),
        (other_dynamic_object->velocity_.y + collisionResult.impact_ * dynamic_object->mass_ * collisionResult.ny_));
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

    rotation_ += angular_velocity_ * timeStep;
    rotation_ = std::fmod(rotation_, 360.f);

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