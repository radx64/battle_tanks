#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "engine/Transform.hpp"

namespace engine
{

class RigidBody
{
public:

    enum class Type
    {
        STATIC,     // Collidable but can't move (fixed position)
        DYNAMIC     // Collidable but can receive momentum (moveable) 
    };

    RigidBody(uint32_t id, Transform& transform, float radius, float mass, float groundDragCooef,
        Type type);
    virtual ~RigidBody() = default;
    void update(float time_step);
    void applyForce(sf::Vector2f force);
    Transform& transform();
    const Transform& transform() const;

public:
    sf::Vector2f velocity_;
    float angularVelocity_;

    uint32_t id_;
    Transform& transform_;
    float radius_;
    float mass_;
    float groundDragCooef_;
    Type type_;
};

}  // namespace engine
