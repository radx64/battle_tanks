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
    RigidBody(uint32_t id, double x, double y, double radius, double mass);
    virtual ~RigidBody() = default;
    void physics(std::vector<std::unique_ptr<RigidBody>>& objects, double timeStep);

public:
    sf::Vector2f velocity_;

    uint32_t id_;
    double x_{};
    double y_{};
    double radius_;
    double mass_;

protected:
    virtual void onPhysics(std::vector<std::unique_ptr<RigidBody>>& objects, double timeStep);
};

}  // namespace game



#endif  // RIGID_BODY_HPP_