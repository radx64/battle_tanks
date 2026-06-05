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
    sf::Sprite sprite_;
    bool needsDisplay_{true};
};

} // namespace game::entity
