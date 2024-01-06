#ifndef RIGID_BODY_HPP_
#define RIGID_BODY_HPP_

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

namespace game {class World;}

namespace game
{

class RigidBody
{
public:

    enum class Type
    {
        STATIC,     // Collidable but can't move (fixed position)
        DYNAMIC     // Collidable but can receive momentum (moveable) 
    };

    RigidBody(uint32_t id, float x, float y, float radius, float mass, float ground_drag_cooef,
        Type type);
    virtual ~RigidBody() = default;
    //void physics(std::vector<std::unique_ptr<RigidBody>>& objects, float timeStep);
    void update(float timeStep);
    void applyForce(sf::Vector2f force);

public:
    sf::Vector2f velocity_;
    float rotation_;
    float angular_velocity_;

    uint32_t id_;
    float x_{};
    float y_{};
    float radius_;
    float mass_;
    float ground_drag_cooef_;
    Type type_;
};

}  // namespace game



#endif  // RIGID_BODY_HPP_