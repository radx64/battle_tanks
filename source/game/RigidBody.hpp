#ifndef RIGID_BODY_HPP_
#define RIGID_BODY_HPP_

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

namespace game
{

class RigidBody
{
public:
    RigidBody(uint32_t id, float x, float y, float radius, float mass, float ground_drag_cooef);
    virtual ~RigidBody() = default;
    void physics(std::vector<std::unique_ptr<RigidBody>>& objects, float timeStep);

public:
    sf::Vector2f velocity_;
    float rotational_velocity_;

    uint32_t id_;
    float x_{};
    float y_{};
    float radius_;
    float mass_;
    float ground_drag_cooef_;

protected:
    virtual void onPhysics(std::vector<std::unique_ptr<RigidBody>>& objects, float timeStep);
    void applyForce(sf::Vector2f force);

};

}  // namespace game



#endif  // RIGID_BODY_HPP_