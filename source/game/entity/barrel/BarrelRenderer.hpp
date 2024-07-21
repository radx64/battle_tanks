#pragma once


#include "engine/Renderer.hpp"

namespace game::entity { class Barrel; }

namespace game::entity
{
class BarrelRenderer : public engine::Renderer
{
public:
    BarrelRenderer(Barrel* barrel, sf::Texture& barrelBody);
    void draw(sf::RenderWindow& renderWindow) override;

protected:
    Barrel* barrel_;
    sf::Sprite barrelSprite_;
};

}  // namespace game::entity 

