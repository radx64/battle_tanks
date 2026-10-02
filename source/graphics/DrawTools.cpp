#include "graphics/DrawTools.hpp"

namespace graphics::drawtools
{

void drawLine(engine::gfx::RenderTarget& window, int x1, int y1, int x2, int y2, engine::gfx::Color color)
{
    engine::gfx::VertexArray line{engine::gfx::PrimitiveType::Lines, 2};
    line[0] = engine::gfx::Vertex(engine::Vec2f(static_cast<float>(x1), static_cast<float>(y1)), color);
    line[1] = engine::gfx::Vertex(engine::Vec2f(static_cast<float>(x2), static_cast<float>(y2)), color);
    window.draw(line);
}


void drawTarget(engine::gfx::RenderTarget& window, int x, int y)
{
    static engine::gfx::CircleShape outerTarget(10.f);
    static engine::gfx::CircleShape innerTarget(2.f);

    outerTarget.setFillColor(engine::gfx::Color(255, 255, 255));
    outerTarget.setOutlineThickness(2.f);
    outerTarget.setOutlineColor(engine::gfx::Color(250, 10, 10));
    outerTarget.setOrigin(10.f, 10.f);
    outerTarget.setPosition(static_cast<float>(x), static_cast<float>(y));
    window.draw(outerTarget);

    innerTarget.setFillColor(engine::gfx::Color(250, 10, 10));
    innerTarget.setOutlineThickness(2.f);
    innerTarget.setOutlineColor(engine::gfx::Color(250, 10, 10));
    innerTarget.setOrigin(2.f, 2.f);
    innerTarget.setPosition(static_cast<float>(x), static_cast<float>(y));
    window.draw(innerTarget);
}

void drawWaypoints(engine::gfx::RenderTarget& window, std::vector<engine::Vec2i>& waypoints)
{
    static engine::gfx::VertexArray lines(engine::gfx::PrimitiveType::Lines);
    lines.clear();
    if (waypoints.size() > 1)
    {
        const auto* lastWaypoint = &waypoints.front();
        for (std::size_t i = 1; i < waypoints.size(); ++i)
        {
            const auto& waypoint = waypoints[i];
            lines.append(engine::gfx::Vertex(engine::Vec2f(static_cast<float>(lastWaypoint->x), static_cast<float>(lastWaypoint->y)), engine::gfx::Color::White));
            lines.append(engine::gfx::Vertex(engine::Vec2f(static_cast<float>(waypoint.x), static_cast<float>(waypoint.y)), engine::gfx::Color::White));
            lastWaypoint = &waypoint;
        }

        if (waypoints.size() > 2)
        {
            lines.append(engine::gfx::Vertex(engine::Vec2f(static_cast<float>(waypoints.front().x), static_cast<float>(waypoints.front().y)), engine::gfx::Color::Red));
            lines.append(engine::gfx::Vertex(engine::Vec2f(static_cast<float>(waypoints.back().x), static_cast<float>(waypoints.back().y)), engine::gfx::Color::Red));
        }
    }

    if (lines.getVertexCount() > 0)
    {
        window.draw(lines);
    }

    for (const auto& waypoint : waypoints)
    {
        drawTarget(window, waypoint.x, waypoint.y);
    }
}

}  // namespeace graphics::drawtools
