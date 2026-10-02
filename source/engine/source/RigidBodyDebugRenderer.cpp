#include "engine/RigidBodyDebugRenderer.hpp"

#include "engine/Scene.hpp"
#include "engine/gfx/Color.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Shape.hpp"
#include "engine/gfx/VertexArray.hpp"

namespace engine
{

namespace
{
void drawLine(engine::gfx::RenderTarget& renderTarget, const engine::Vec2f& from, const engine::Vec2f& to,
    const engine::gfx::Color& color)
{
    engine::gfx::VertexArray line{engine::gfx::PrimitiveType::Lines, 2};
    line[0] = engine::gfx::Vertex{from, color};
    line[1] = engine::gfx::Vertex{to, color};
    renderTarget.draw(line);
}
}  // namespace

void RigidBodyDebugRenderer::debug(engine::Scene& scene, engine::gfx::RenderTarget& renderTarget)
{
    for (const auto& gameObject : scene.objects())
    {
        const auto x = gameObject->getRigidBody().transform().position().x;
        const auto y = gameObject->getRigidBody().transform().position().y;
        const float radius = gameObject->getRigidBody().radius_;

        engine::gfx::CircleShape boundary{radius, 12};
        boundary.setFillColor(engine::gfx::Color{0, 0, 0, 0});
        boundary.setOutlineThickness(1.f);
        boundary.setOutlineColor(gameObject->getRigidBody().type_ == engine::RigidBody::Type::STATIC
            ? engine::gfx::Color::Blue
            : engine::gfx::Color::Red);
        boundary.setOrigin({radius, radius});
        boundary.setPosition({x, y});
        renderTarget.draw(boundary);

        const auto& velocity = gameObject->getRigidBody().velocity_;
        drawLine(renderTarget, {x, y}, {x + velocity.x, y + velocity.y}, engine::gfx::Color::White);
        drawLine(renderTarget, {x, y}, {x + velocity.x, y}, engine::gfx::Color::Red);
        drawLine(renderTarget, {x, y}, {x, y + velocity.y}, engine::gfx::Color::Green);
    }
}

}  // namespace engine
