#ifndef GAME_ENTITY_TANK_RENDERER_HPP_
#define GAME_ENTITY_TANK_RENDERER_HPP_

#include "game/Renderer.hpp"

namespace game::entity { class Tank; }

namespace game::entity
{
class TankRenderer : public Renderer
{
public:
    TankRenderer(Tank* tank, sf::Texture& tankBody);
    void draw(sf::RenderWindow& renderWindow);
    void drawDebugInfo(sf::RenderWindow& renderWindow);

protected:
    Tank* tank_;
    sf::Sprite tankSprite_;
};

}  // namespace game::entity 

#endif  // GAME_ENTITY_TANK_RENDERER_HPP_