#pragma once

#include <SFML/Graphics.hpp>

namespace game::entity
{

class TracksRenderer
{
public:
    TracksRenderer();
    void addTrackImprint(const int x, const int y, const float angle);
    void draw(sf::RenderWindow& renderWindow);
    void clear();

protected:
    sf::RenderTexture target_;
};

} // namespace game::entity

