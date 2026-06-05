#include "graphics/DrawTools.hpp"

namespace graphics::drawtools
{

void drawLine(sf::RenderWindow& window, int x1, int y1, int x2, int y2, sf::Color color)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1, y1), color),
        sf::Vertex(sf::Vector2f(x2, y2), color)
    };

    window.draw(line, 2, sf::Lines);    
}


void drawTarget(sf::RenderWindow& window, int x, int y)
{
    static sf::CircleShape outerTarget(10.f);
    static sf::CircleShape innerTarget(2.f);

    outerTarget.setFillColor(sf::Color(255, 255, 255));
    outerTarget.setOutlineThickness(2.f);
    outerTarget.setOutlineColor(sf::Color(250, 10, 10));
    outerTarget.setOrigin(10.f, 10.f);
    outerTarget.setPosition(static_cast<float>(x), static_cast<float>(y));
    window.draw(outerTarget);

    innerTarget.setFillColor(sf::Color(250, 10, 10));
    innerTarget.setOutlineThickness(2.f);
    innerTarget.setOutlineColor(sf::Color(250, 10, 10));
    innerTarget.setOrigin(2.f, 2.f);
    innerTarget.setPosition(static_cast<float>(x), static_cast<float>(y));
    window.draw(innerTarget);
}

void drawWaypoints(sf::RenderWindow& window, std::vector<sf::Vector2i>& waypoints)
{
    static sf::VertexArray lines(sf::Lines);
    lines.clear();
    if (waypoints.size() > 1)
    {
        const auto* lastWaypoint = &waypoints.front();
        for (std::size_t i = 1; i < waypoints.size(); ++i)
        {
            const auto& waypoint = waypoints[i];
            lines.append(sf::Vertex(sf::Vector2f(static_cast<float>(lastWaypoint->x), static_cast<float>(lastWaypoint->y)), sf::Color::White));
            lines.append(sf::Vertex(sf::Vector2f(static_cast<float>(waypoint.x), static_cast<float>(waypoint.y)), sf::Color::White));
            lastWaypoint = &waypoint;
        }

        if (waypoints.size() > 2)
        {
            lines.append(sf::Vertex(sf::Vector2f(static_cast<float>(waypoints.front().x), static_cast<float>(waypoints.front().y)), sf::Color::Red));
            lines.append(sf::Vertex(sf::Vector2f(static_cast<float>(waypoints.back().x), static_cast<float>(waypoints.back().y)), sf::Color::Red));
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
