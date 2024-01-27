#ifndef GAME_ENTITY_TRACKSRENDERER_HPP_
#define GAME_ENTITY_TRACKSRENDERER_HPP_

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

#endif  //  GAME_ENTITY_TRACKSRENDERER_HPP_