#ifndef GAME_BULLET_RENDERER_HPP_
#define GAME_BULLET_RENDERER_HPP_

#include "game/Renderer.hpp"

namespace game { class Bullet; }

namespace game
{
class BulletRenderer : public Renderer
{
public:
    BulletRenderer(Bullet* bullet, sf::Texture& bulletBody);
    void draw(sf::RenderWindow& renderWindow);

protected:
    Bullet* bullet_;
    sf::Sprite bulletSprite_;
};

}  // namespace game 

#endif  // GAME_BULLET_RENDERER_HPP_