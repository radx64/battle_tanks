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

CollisionResult processObjectsCollsion(engine::RigidBody& object, engine::RigidBody& otherObject)
{
   float distanceBetweenObjects = engine::math::distance(object.x_, object.y_, 
        otherObject.x_, otherObject.y_);
    // Collision detected here
    // Calculating hit vectors
    float nx = (otherObject.x_ - object.x_) / distanceBetweenObjects;
    float ny = (otherObject.y_ - object.y_) / distanceBetweenObjects;
    float impact = SPRING_COLLISION_COEEF * (
        engine::math::dotProduct(object.velocity_.x, object.velocity_.y, nx, ny)
        - engine::math::dotProduct(otherObject.velocity_.x, otherObject.velocity_.y, nx, ny)) 
        / (object.mass_ + otherObject.mass_);

    // Additional angular velocity based on the point of collision
    // So objects rotate when hit
    auto relativeVelocity = object.velocity_ - otherObject.velocity_;
    float velocityVectorLength = engine::math::normalizeVector(relativeVelocity);
    float dotProduct = engine::math::dotProduct(nx, ny, relativeVelocity.x, relativeVelocity.y);
    // TODO: Consider adding mass and impact momentum to this tangent velocity later
    float tangentVelocity =  velocityVectorLength * dotProduct * TANGENT_VELOCITY_SCALING_FACTOR;

    // This cross product is used to determine rotation direction of the angular momentum    
    float crossProduct = engine::math::crossProduct(
        object.velocity_.x, 
        object.velocity_.y,
        otherObject.velocity_.x, 
        otherObject.velocity_.y);

    if (crossProduct > 0) tangentVelocity = tangentVelocity * -1.f;

    return CollisionResult{
        .tangentVelocity_ = tangentVelocity, 
        .impact_ = impact, 
        .nx_ = nx,
        .ny_ = ny};
}

void processStaticAndDynamicObjectsCollsion(engine::RigidBody& staticObject, engine::RigidBody& dynamicObject)
{
    auto collisionResult = processObjectsCollsion(staticObject, dynamicObject);

    dynamicObject.angularVelocity_ -= collisionResult.tangentVelocity_ / dynamicObject.radius_;

    sf::Vector2f normalVector{collisionResult.nx_,collisionResult.ny_};
    engine::math::normalizeVector(normalVector);

    float relativeSpeed = engine::math::dotProduct(
        dynamicObject.velocity_.x, dynamicObject.velocity_.y,
        normalVector.x, normalVector.y);

    float impulse = 2.0f * relativeSpeed;

    // FIXME: There is some bug in above formula for small radiuses on low speeds
    // dynamic object can stick to static object instead of bounce off
    // I need to investigate it further.
    dynamicObject.velocity_ = dynamicObject.velocity_ - normalVector * impulse ;
}

void processDynamicObjectsCollsion(engine::RigidBody& dynamicObject, engine::RigidBody& otherDynamicObject)
{ 
    auto collisionResult = processObjectsCollsion(dynamicObject, otherDynamicObject);

    dynamicObject.angularVelocity_ += collisionResult.tangentVelocity_ / dynamicObject.radius_;
    otherDynamicObject.angularVelocity_ -= collisionResult.tangentVelocity_  / otherDynamicObject.radius_;

    sf::Vector2f objectCollisionVelocity = sf::Vector2f(
        (dynamicObject.velocity_.x - collisionResult.impact_ * otherDynamicObject.mass_ * collisionResult.nx_),
        (dynamicObject.velocity_.y - collisionResult.impact_ * otherDynamicObject.mass_ * collisionResult.ny_));
    
    dynamicObject.velocity_ = objectCollisionVelocity;

    sf::Vector2f otherObjectCollisionVelocity = sf::Vector2f(
        (otherDynamicObject.velocity_.x + collisionResult.impact_ * dynamicObject.mass_ * collisionResult.nx_),
        (otherDynamicObject.velocity_.y + collisionResult.impact_ * dynamicObject.mass_ * collisionResult.ny_));
    otherDynamicObject.velocity_ = otherObjectCollisionVelocity;
}

void solveCollsion(engine::GameObject& object, engine::GameObject& otherObject)
{
    auto& rigidBody = object.getRigidBody();
    auto& otherRigidBody = otherObject.getRigidBody();
    if (rigidBody.id_ == otherRigidBody.id_) return; // do not check collision with itself.
    
    float distanceBetweenObjects = engine::math::distance(rigidBody.x_, rigidBody.y_,
        otherRigidBody.x_, otherRigidBody.y_);

    float sumOfRadius = rigidBody.radius_ + otherRigidBody.radius_;

    if (distanceBetweenObjects >= sumOfRadius)
    {
        return;  // no collsion
    }

    float objectsOverlap = (distanceBetweenObjects - rigidBody.radius_ - otherRigidBody.radius_);

    if (rigidBody.type_ == engine::RigidBody::Type::STATIC)
    {
        if (otherRigidBody.type_ == engine::RigidBody::Type::STATIC)
        {
            // Collision of static objects should not do anything
            // TODO: call some onCollision handler on game objects
        } 
        else /* other object Type::Dynamic */
        {
            processStaticAndDynamicObjectsCollsion(rigidBody, otherRigidBody);

            otherRigidBody.x_ += objectsOverlap * (rigidBody.x_ - otherRigidBody.x_) / distanceBetweenObjects;
            otherRigidBody.y_ += objectsOverlap * (rigidBody.y_ - otherRigidBody.y_) / distanceBetweenObjects;
        }

    } 
    else /* Type::Dynamic */
    {
        if (otherRigidBody.type_ == engine::RigidBody::Type::STATIC)
        {
            processStaticAndDynamicObjectsCollsion(otherRigidBody, rigidBody);
            
            rigidBody.x_ -= objectsOverlap * (rigidBody.x_ - otherRigidBody.x_) / distanceBetweenObjects;
            rigidBody.y_ -= objectsOverlap * (rigidBody.y_ - otherRigidBody.y_) / distanceBetweenObjects;
        } 
        else /* other object Type::Dynamic */
        {
            processDynamicObjectsCollsion(rigidBody, otherRigidBody);

            rigidBody.x_ -= 0.5f * objectsOverlap * (rigidBody.x_ - otherRigidBody.x_) / distanceBetweenObjects;
            rigidBody.y_ -= 0.5f * objectsOverlap * (rigidBody.y_ - otherRigidBody.y_) / distanceBetweenObjects;

            otherRigidBody.x_ += 0.5f * objectsOverlap * (rigidBody.x_ - otherRigidBody.x_) / distanceBetweenObjects;
            otherRigidBody.y_ += 0.5f * objectsOverlap * (rigidBody.y_ - otherRigidBody.y_) / distanceBetweenObjects;
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
