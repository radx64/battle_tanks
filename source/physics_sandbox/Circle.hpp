#ifndef SANDBOX_CIRCLE_HPP_
#define SANDBOX_CIRCLE_HPP_

#include "engine/GameObject.hpp"

constexpr float DRAG_COEEF = 0.95f;

namespace sandbox
{
class Circle : public engine::GameObject
{
public:
Circle(uint32_t id, float x, float y, float radius, float mass,
const engine::RigidBody::Type& type)
{
    renderer_ = nullptr;
    rigid_body_ = std::make_unique<engine::RigidBody>(
        id,
        x, y, radius,
        mass,
        DRAG_COEEF,
        type
    );
}

};

}  // namespace sandbox

#endif  // SANDBOX_CIRCLE_HPP_