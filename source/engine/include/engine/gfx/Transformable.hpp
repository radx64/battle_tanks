#pragma once

#include "engine/Vector2.hpp"

namespace engine::gfx
{

class Transformable
{
public:
    void setPosition(const engine::Vec2f& position) { position_ = position; }
    const engine::Vec2f& getPosition() const { return position_; }
    void setOrigin(const engine::Vec2f& origin) { origin_ = origin; }
    const engine::Vec2f& getOrigin() const { return origin_; }

protected:
    engine::Vec2f position_{};
    engine::Vec2f origin_{};
};

}  // namespace engine::gfx
