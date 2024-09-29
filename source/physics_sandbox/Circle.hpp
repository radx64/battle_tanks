#pragma once

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
    rigidBody_ = std::make_unique<engine::RigidBody>(
        id,
        x, y, radius,
        mass,
        DRAG_COEEF,
        type
    );
}

};

}  // namespace sandbox
