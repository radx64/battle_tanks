#pragma once

#include <SFML/Graphics.hpp>

#include "engine/ChunkedRenderTextureLayer.hpp"

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
    engine::ChunkedRenderTextureLayer chunks_;
};

} // namespace game::entity
