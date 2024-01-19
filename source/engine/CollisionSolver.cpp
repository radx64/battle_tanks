#include "engine/CollisionSolver.hpp"

#include <cmath>

#include "math/Math.hpp"
#include "engine/Scene.hpp"

namespace engine
{

namespace
{

constexpr float SPRING_COLLISION_COEEF = 1.75;  // simulate bouciness of collision

constexpr float TANGENT_VELOCITY_SCALING_FACTOR = 25.f;

struct CollisionResult
{
    float tangentVelocity_;     // velocity added to angular velocity of an object
    float impact_;              // linear collision force
    float nx_;                  // normal hit vector x coordinate
    float ny_;                  // normal hit vector y coordinate
};

CollisionResult processObjectsCollsion(engine::RigidBody& object, engine::RigidBody& other_object)
{
   float distance_between_objects = math::distance(object.x_, object.y_, 
        other_object.x_, other_object.y_);
    // Collision detected here
    // Calculating hit vectors
    float nx = (other_object.x_ - object.x_) / distance_between_objects;
    float ny = (other_object.y_ - object.y_) / distance_between_objects;
    float impact = SPRING_COLLISION_COEEF * (
        math::dot_product(object.velocity_.x, object.velocity_.y, nx, ny)
        - math::dot_product(other_object.velocity_.x, other_object.velocity_.y, nx, ny)) 
        / (object.mass_ + other_object.mass_);

    // Additional angular velocity based on the point of collision
    // So objects rotate when hit
    auto relativeVelocity = object.velocity_ - other_object.velocity_;
    float velocityVectorLength = math::normalize_vector(relativeVelocity);
    float dotProduct = math::dot_product(nx, ny, relativeVelocity.x, relativeVelocity.y);
    // TODO: Consider adding mass and impact momentum to this tangent velocity later
    float tangentVelocity =  velocityVectorLength * dotProduct * TANGENT_VELOCITY_SCALING_FACTOR;

    // This cross product is used to determine rotation direction of the angular momentum    
    float crossProduct = math::cross_product(
        object.velocity_.x, 
        object.velocity_.y,
        other_object.velocity_.x, 
        other_object.velocity_.y);

    if (crossProduct > 0) tangentVelocity = tangentVelocity * -1.f;

    return CollisionResult{
        .tangentVelocity_ = tangentVelocity, 
        .impact_ = impact, 
        .nx_ = nx,
        .ny_ = ny};
}

void processStaticAndDynamicObjectsCollsion(engine::RigidBody& static_object, engine::RigidBody& dynamic_object)
{
    auto collisionResult = processObjectsCollsion(static_object, dynamic_object);

    dynamic_object.angular_velocity_ -= collisionResult.tangentVelocity_ / dynamic_object.radius_;

    sf::Vector2f normalVector{collisionResult.nx_,collisionResult.ny_};
    math::normalize_vector(normalVector);

    float relativeSpeed = math::dot_product(
        dynamic_object.velocity_.x, dynamic_object.velocity_.y,
        normalVector.x, normalVector.y);

    float impulse = 2.0f * relativeSpeed;

    // FIXME: There is some bug in above formula for small radiuses on low speeds
    // dynamic object can stick to static object instead of bounce off
    // I need to investigate it further.
    dynamic_object.velocity_ = dynamic_object.velocity_ - normalVector * impulse ;
}

void processDynamicObjectsCollsion(engine::RigidBody& dynamic_object, engine::RigidBody& other_dynamic_object)
{ 
    auto collisionResult = processObjectsCollsion(dynamic_object, other_dynamic_object);

    dynamic_object.angular_velocity_ += collisionResult.tangentVelocity_ / dynamic_object.radius_;
    other_dynamic_object.angular_velocity_ -= collisionResult.tangentVelocity_  / other_dynamic_object.radius_;

    sf::Vector2f objectCollisionVelocity = sf::Vector2f(
        (dynamic_object.velocity_.x - collisionResult.impact_ * other_dynamic_object.mass_ * collisionResult.nx_),
        (dynamic_object.velocity_.y - collisionResult.impact_ * other_dynamic_object.mass_ * collisionResult.ny_));
    
    dynamic_object.velocity_ = objectCollisionVelocity;

    sf::Vector2f otherObjectCollisionVelocity = sf::Vector2f(
        (other_dynamic_object.velocity_.x + collisionResult.impact_ * dynamic_object.mass_ * collisionResult.nx_),
        (other_dynamic_object.velocity_.y + collisionResult.impact_ * dynamic_object.mass_ * collisionResult.ny_));
    other_dynamic_object.velocity_ = otherObjectCollisionVelocity;
}

void solveCollsion(engine::GameObject& object, engine::GameObject& other_object)
{
    auto& rigid_body = object.getRigidBody();
    auto& other_rigid_body = other_object.getRigidBody();
    if (rigid_body.id_ == other_rigid_body.id_) return; // do not check collision with itself.
    
    float distance_between_objects = math::distance(rigid_body.x_, rigid_body.y_,
        other_rigid_body.x_, other_rigid_body.y_);

    float sum_of_radius = rigid_body.radius_ + other_rigid_body.radius_;

    if (distance_between_objects >= sum_of_radius)
    {
        return;  // no collsion
    }

    float objects_overlap = (distance_between_objects - rigid_body.radius_ - other_rigid_body.radius_);

    if (rigid_body.type_ == engine::RigidBody::Type::STATIC)
    {
        if (other_rigid_body.type_ == engine::RigidBody::Type::STATIC)
        {
            // Collision of static objects should not do anything
            // TODO: call some onCollision handler on game objects
        } 
        else /* other object Type::Dynamic */
        {
            processStaticAndDynamicObjectsCollsion(rigid_body, other_rigid_body);

            other_rigid_body.x_ += objects_overlap * (rigid_body.x_ - other_rigid_body.x_) / distance_between_objects;
            other_rigid_body.y_ += objects_overlap * (rigid_body.y_ - other_rigid_body.y_) / distance_between_objects;
        }

    } 
    else /* Type::Dynamic */
    {
        if (other_rigid_body.type_ == engine::RigidBody::Type::STATIC)
        {
            processStaticAndDynamicObjectsCollsion(other_rigid_body, rigid_body);
            
            rigid_body.x_ -= objects_overlap * (rigid_body.x_ - other_rigid_body.x_) / distance_between_objects;
            rigid_body.y_ -= objects_overlap * (rigid_body.y_ - other_rigid_body.y_) / distance_between_objects;
        } 
        else /* other object Type::Dynamic */
        {
            processDynamicObjectsCollsion(rigid_body, other_rigid_body);

            rigid_body.x_ -= 0.5f * objects_overlap * (rigid_body.x_ - other_rigid_body.x_) / distance_between_objects;
            rigid_body.y_ -= 0.5f * objects_overlap * (rigid_body.y_ - other_rigid_body.y_) / distance_between_objects;

            other_rigid_body.x_ += 0.5f * objects_overlap * (rigid_body.x_ - other_rigid_body.x_) / distance_between_objects;
            other_rigid_body.y_ += 0.5f * objects_overlap * (rigid_body.y_ - other_rigid_body.y_) / distance_between_objects;
        }
    }
}

}  // namespace

CollisionSolver::CollisionSolver(engine::Scene& scene)
: scene_(scene)
{}


void CollisionSolver::evaluateCollisions()
{
    auto& gameObjects = scene_.objects();
    for (size_t i = 0; i < gameObjects.size(); ++i)
    {
        for (size_t j = i + 1; j < gameObjects.size(); ++j) 
        {
            solveCollsion(*gameObjects[i], *gameObjects[j]);
        }
    }
}

}  // namespace engine