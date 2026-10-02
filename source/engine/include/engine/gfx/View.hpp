#pragma once

#include "engine/Vector2.hpp"

namespace engine::gfx
{

class View
{
public:
    View() = default;
    View(const engine::Vec2f& center, const engine::Vec2f& size);

    void setCenter(const engine::Vec2f& center);
    void setSize(const engine::Vec2f& size);
    const engine::Vec2f& getCenter() const;
    const engine::Vec2f& getSize() const;

    void zoom(float factor);
    void move(const engine::Vec2f& offset);

private:
    engine::Vec2f center_{};
    engine::Vec2f size_{};
};

}  // namespace engine::gfx
