#pragma once

#include "engine/Vector2.hpp"

namespace engine
{

class Transform
{
public:
    Transform() = default;
    ~Transform() = default;

    engine::Vector2f& position() { return position_; }
    const engine::Vector2f& position() const { return position_; }

    engine::Vector2f& scale() { return scale_; }
    const engine::Vector2f& scale() const { return scale_; }

    float& rotation() { return rotation_; }
    const float& rotation() const { return rotation_; }

protected:
    engine::Vector2f position_{0.f, 0.f};
    engine::Vector2f scale_{1.f, 1.f};
    float rotation_{0.f};
};

}  // namespace engine
