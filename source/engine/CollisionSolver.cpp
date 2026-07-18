#include "engine/CollisionSolver.hpp"

#include <cmath>

#include "engine/math/Math.hpp"
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
   float distance_between_objects = engine::math::distance(object.transform().position().x, object.transform().position().y, 
        other_object.transform().position().x, other_object.transform().position().y);
    // Collision detected here
    // Calculating hit vectors
    float nx = (other_object.transform().position().x - object.transform().position().x) / distance_between_objects;
    float ny = (other_object.transform().position().y - object.transform().position().y) / distance_between_objects;
    float impact = SPRING_COLLISION_COEEF * (
        engine::math::dotProduct(object.velocity_.x, object.velocity_.y, nx, ny)
        - engine::math::dotProduct(other_object.velocity_.x, other_object.velocity_.y, nx, ny)) 
        / (object.mass_ + other_object.mass_);

    // Additional angular velocity based on the point of collision
    // So objects rotate when hit
    auto relativeVelocity = object.velocity_ - other_object.velocity_;
    float velocityVectorLength = engine::math::normalizeVector(relativeVelocity);
    float dotProduct = engine::math::dotProduct(nx, ny, relativeVelocity.x, relativeVelocity.y);
    // TODO: Consider adding mass and impact momentum to this tangent velocity later
    float tangentVelocity =  velocityVectorLength * dotProduct * TANGENT_VELOCITY_SCALING_FACTOR;

    // This cross product is used to determine rotation direction of the angular momentum    
    float crossProduct = engine::math::crossProduct(
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

void processStaticAndDynamicObjectsCollsion(engine::RigidBody& staticObject, engine::RigidBody& dynamic_object)
{
    auto collisionResult = processObjectsCollsion(staticObject, dynamic_object);

    dynamic_object.angularVelocity_ -= collisionResult.tangentVelocity_ / dynamic_object.radius_;

    sf::Vector2f normalVector{collisionResult.nx_,collisionResult.ny_};
    engine::math::normalizeVector(normalVector);

    float relativeSpeed = engine::math::dotProduct(
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

    dynamic_object.angularVelocity_ += collisionResult.tangentVelocity_ / dynamic_object.radius_;
    other_dynamic_object.angularVelocity_ -= collisionResult.tangentVelocity_  / other_dynamic_object.radius_;

    sf::Vector2f object_collision_velocity = sf::Vector2f(
        (dynamic_object.velocity_.x - collisionResult.impact_ * other_dynamic_object.mass_ * collisionResult.nx_),
        (dynamic_object.velocity_.y - collisionResult.impact_ * other_dynamic_object.mass_ * collisionResult.ny_));
    
    dynamic_object.velocity_ = object_collision_velocity;

    sf::Vector2f other_object_collision_velocity = sf::Vector2f(
        (other_dynamic_object.velocity_.x + collisionResult.impact_ * dynamic_object.mass_ * collisionResult.nx_),
        (other_dynamic_object.velocity_.y + collisionResult.impact_ * dynamic_object.mass_ * collisionResult.ny_));
    other_dynamic_object.velocity_ = other_object_collision_velocity;
}

void solveCollsion(engine::GameObject& object, engine::GameObject& other_object)
{
    auto& rigid_body = object.getRigidBody();
    auto& other_rigid_body = other_object.getRigidBody();
    if (rigid_body.id_ == other_rigid_body.id_) return; // do not check collision with itself.
    
    float distance_between_objects = engine::math::distance(rigid_body.transform().position().x, rigid_body.transform().position().y,
        other_rigid_body.transform().position().x, other_rigid_body.transform().position().y);

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

            auto& position = rigid_body.transform().position();
            auto& other_position = other_rigid_body.transform().position();

            other_position.x += objects_overlap * (position.x - other_position.x) / distance_between_objects;
            other_position.y += objects_overlap * (position.y - other_position.y) / distance_between_objects;
        }

    } 
    else /* Type::Dynamic */
    {
        if (other_rigid_body.type_ == engine::RigidBody::Type::STATIC)
        {
            processStaticAndDynamicObjectsCollsion(other_rigid_body, rigid_body);
            
            auto& position = rigid_body.transform().position();
            auto& other_position = other_rigid_body.transform().position();
            position.x -= objects_overlap * (position.x - other_position.x) / distance_between_objects;
            position.y -= objects_overlap * (position.y - other_position.y) / distance_between_objects;
        } 
        else /* other object Type::Dynamic */
        {
            processDynamicObjectsCollsion(rigid_body, other_rigid_body);

            auto& position = rigid_body.transform().position();
            auto& other_position = other_rigid_body.transform().position();

            position.x -= 0.5f * objects_overlap * (position.x - other_position.x) / distance_between_objects;
            position.y -= 0.5f * objects_overlap * (position.y - other_position.y) / distance_between_objects;

            other_position.x += 0.5f * objects_overlap * (position.x - other_position.x) / distance_between_objects;
            other_position.y += 0.5f * objects_overlap * (position.y - other_position.y) / distance_between_objects;
        }
    }
}

}  // namespace

CollisionSolver::CollisionSolver(engine::Scene& scene)
: scene_(scene)
{}


void CollisionSolver::evaluateCollisions()
{
    auto& game_objects = scene_.objects();
    for (size_t i = 0; i < game_objects.size(); ++i)
    {
        for (size_t j = i + 1; j < game_objects.size(); ++j) 
        {
            solveCollsion(*game_objects[i], *game_objects[j]);
        }
    }
}

}  // namespace engine
