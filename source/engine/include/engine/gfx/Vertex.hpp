#pragma once

#include "engine/gfx/Color.hpp"
#include "engine/Vector2.hpp"

namespace engine::gfx
{

class Vertex
{

public:
    Vertex() = default;

    Vertex(const engine::Vec2f& position);
    Vertex(const engine::Vec2f& position, const engine::gfx::Color& color);
    Vertex(const engine::Vec2f& position, const engine::gfx::Color& color, const engine::Vec2f& uv);

    engine::Vec2f position;
    engine::gfx::Color color{255, 255, 255, 255};
    engine::Vec2f uv;
};

}  // namespace engine::gfx
