#ifndef GAME_ENTITY_BULLET_BULLET_RENDERER_HPP_
#define GAME_ENTITY_BULLET_BULLET_RENDERER_HPP_

#include "engine/Renderer.hpp"

namespace game::entity { class Bullet; }

namespace game::entity
{
class BulletRenderer : public engine::Renderer
{
public:
    BulletRenderer(Bullet* bullet, sf::Texture& bulletBody);
    void draw(sf::RenderWindow& renderWindow) override;

protected:
    Bullet* bullet_;
    sf::Sprite bulletSprite_;
};

}  // namespace game::entity 

#endif  // GAME_ENTITY_BULLET_BULLET_RENDERER_HPP_