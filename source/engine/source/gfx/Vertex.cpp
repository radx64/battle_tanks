#include "engine/gfx/Vertex.hpp"

namespace engine::gfx
{

Vertex::Vertex(const engine::Vec2f& position)
: position(position), color(Color::White), uv({0.f, 0.f})
{}

Vertex::Vertex(const engine::Vec2f& position, const engine::gfx::Color& color)
: position(position), color(color), uv({0.f, 0.f})
{}

Vertex::Vertex(const engine::Vec2f& position, const engine::gfx::Color& color, const engine::Vec2f& uv)
: position(position), color(color), uv(uv)
{}    

}  // namespace engine::gfx
