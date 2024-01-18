#ifndef GAME_ENTITY_BARREL_BARREL_RENDERER_HPP_
#define GAME_ENTITY_BARREL_BARREL_RENDERER_HPP_

#include "game/Renderer.hpp"

namespace game::entity { class Barrel; }

namespace game::entity
{
class BarrelRenderer : public Renderer
{
public:
    BarrelRenderer(Barrel* barrel, sf::Texture& barrelBody);
    void draw(sf::RenderWindow& renderWindow) override;

protected:
    Barrel* barrel_;
    sf::Sprite barrelSprite_;
};

}  // namespace game::entity 

#endif  // GAME_ENTITY_BARREL_BARREL_RENDERER_HPP_