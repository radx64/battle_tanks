#pragma once


#include "engine/GameObject.hpp"

constexpr float DRAG_COEEF = 0.95f;

namespace sandbox
{
class Box : public engine::GameObject
{
public:
Box(uint32_t id, float x, float y, float width, float height, float mass,
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

