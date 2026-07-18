#pragma once

#include "engine/Renderer.hpp"

namespace game::entity { class Tank; }

namespace game::entity
{
class TankRenderer : public engine::Renderer
{
public:
    TankRenderer(Tank* tank, sf::Texture& tankBody);
    void draw(sf::RenderWindow& render_window);
    void drawDebugInfo(sf::RenderWindow& render_window);

protected:
    Tank* tank_;
    sf::Sprite tankSprite_;
};

}  // namespace game::entity 
