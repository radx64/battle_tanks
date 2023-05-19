#include "DrawTools.hpp"

namespace drawtools
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
    sf::CircleShape target(10);
    target.setFillColor(sf::Color(255, 255, 255));
    target.setOutlineThickness(2);
    target.setOutlineColor(sf::Color(250, 10, 10));
    target.setPosition(x, y);
    target.setOrigin(10,10);
    window.draw(target);

    sf::CircleShape target2(2);
    target2.setFillColor(sf::Color(250, 10, 10));
    target2.setOutlineThickness(2);
    target2.setOutlineColor(sf::Color(250, 10, 10));
    target2.setPosition(x, y);
    target2.setOrigin(2,2);
    window.draw(target2);
}

void drawWaypoints(sf::RenderWindow& window, std::vector<sf::Vector2i>& waypoints)
{
    const sf::Vector2i* last_waypoint = nullptr;
    for(const auto& waypoint : waypoints)
    {
        drawTarget(window, waypoint.x, waypoint.y);
        if(last_waypoint)
        {
            drawLine(window, last_waypoint->x, last_waypoint->y, waypoint.x, waypoint.y, sf::Color::White);
        }
        last_waypoint = &waypoint;
    }

    if (waypoints.size() > 2) 
    {
        drawLine(window, waypoints.front().x, 
            waypoints.front().y, 
            waypoints.back().x, 
            waypoints.back().y,
            sf::Color::Red);
    }
}

}  // namespeace drawtools