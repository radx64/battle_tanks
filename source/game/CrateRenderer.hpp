#ifndef GAME_CRATE_RENDERER_HPP_
#define GAME_CRATE_RENDERER_HPP_

#include "game/Renderer.hpp"

namespace game { class Crate; }

namespace game
{
class CrateRenderer : public Renderer
{
public:
    CrateRenderer(Crate* crate, sf::Texture& crateBody);
    void draw(sf::RenderWindow& renderWindow);

protected:
    Crate* crate_;
    sf::Sprite crateSprite_;
};


}  // namespace game 

#endif  // GAME_CRATE_RENDERER_HPP_