#ifndef DRAW_TOOLS_HPP_
#define DRAW_TOOLS_HPP_

#include <vector>

#include <SFML/Graphics.hpp>

namespace drawtools
{

void drawLine(sf::RenderWindow& window, int x1, int y1, int x2, int y2, sf::Color color);
void drawTarget(sf::RenderWindow& window, int x, int y);
void drawWaypoints(sf::RenderWindow& window, std::vector<sf::Vector2i>& waypoints);

}  // namespeace drawtools

#endif  // DRAW_TOOLS_HPP_
