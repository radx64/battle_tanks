#pragma once

#include "engine/Renderer.hpp"

namespace game::entity { class Crate; }

namespace game::entity
{
class CrateRenderer : public engine::Renderer
{
public:
    CrateRenderer(Crate* crate, sf::Texture& crateBody);
    void draw(sf::RenderWindow& renderWindow) override;

protected:
    Crate* crate_;
    sf::Sprite crateSprite_;
};


}  // namespace game::entity 

