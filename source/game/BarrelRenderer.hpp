#ifndef GAME_BARREL_RENDERER_HPP_
#define GAME_BARREL_RENDERER_HPP_

#include "game/Renderer.hpp"

namespace game { class Barrel; }

namespace game
{
class BarrelRenderer : public Renderer
{
public:
    BarrelRenderer(Barrel* barrel, sf::Texture& barrelBody);
    void draw(sf::RenderWindow& renderWindow);

protected:
    Barrel* barrel_;
    sf::Sprite barrelSprite_;
};


}  // namespace game 

#endif  // GAME_BARREL_RENDERER_HPP_