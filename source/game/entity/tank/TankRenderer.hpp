#pragma once

#include "engine/Renderer.hpp"

namespace game::entity { class Tank; }

namespace game::entity
{
class TankRenderer : public engine::Renderer
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

