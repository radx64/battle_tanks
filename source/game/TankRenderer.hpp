#ifndef GAME_TANK_RENDERER_HPP_
#define GAME_TANK_RENDERER_HPP_

#include "game/Renderer.hpp"

namespace game { class Tank; }

namespace game
{
class TankRenderer : public Renderer
{
public:
    TankRenderer(Tank* tank, sf::Texture& tankBody);
    void draw(sf::RenderWindow& renderWindow);

protected:
    Tank* tank_;
    sf::Sprite tankSprite_;
};


}  // namespace game 

#endif  // GAME_TANK_RENDERER_HPP_