#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

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

    RigidBody(uint32_t id, float x, float y, float radius, float mass, float groundDragCooef,
        Type type);
    virtual ~RigidBody() = default;
    void update(float timeStep);
    void applyForce(sf::Vector2f force);

public:
    sf::Vector2f velocity_;
    float rotation_;
    float angularVelocity_;

    uint32_t id_;
    float x_{};
    float y_{};
    float radius_;
    float mass_;
    float groundDragCooef_;
    Type type_;
};

}  // namespace engine
