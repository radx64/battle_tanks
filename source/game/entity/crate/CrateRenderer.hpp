#ifndef GAME_CRATE_RENDERER_HPP_
#define GAME_CRATE_RENDERER_HPP_

#include "game/Renderer.hpp"

namespace game::entity { class Crate; }

namespace game::entity
{
class CrateRenderer : public Renderer
{
public:
    CrateRenderer(Crate* crate, sf::Texture& crateBody);
    void draw(sf::RenderWindow& renderWindow) override;

protected:
    Crate* crate_;
    sf::Sprite crateSprite_;
};


}  // namespace game::entity 

#endif  // GAME_CRATE_RENDERER_HPP_