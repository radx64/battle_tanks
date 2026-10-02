#include "game/entity/bullet/BulletRenderer.hpp"
#include "game/entity/bullet/Bullet.hpp"

namespace game::entity
{

BulletRenderer::BulletRenderer(Bullet* bullet, engine::gfx::Texture& bulletBody)
: bullet_(bullet)
{
    bulletSprite_.setTexture(bulletBody);
    engine::Vec2u texture_body_size = bulletBody.getSize();
    auto middle_point = engine::Vec2f(texture_body_size.x / 2, texture_body_size.y / 2);
    bulletSprite_.setOrigin(middle_point);     
}

void BulletRenderer::draw(engine::gfx::RenderTarget& render_window)
{
    auto& bulletTransform = bullet_->transform();

    bulletSprite_.setRotation(bulletTransform.rotation());
    bulletSprite_.setColor(engine::gfx::Color(10, 10, 10, 127));
    bulletSprite_.setPosition(bulletTransform.position().x + 4, bulletTransform.position().y + 4);
   render_window.draw(bulletSprite_);

    bulletSprite_.setColor(engine::gfx::Color(255, 255, 255, 255));
    bulletSprite_.setPosition(bulletTransform.position().x, bulletTransform.position().y);
   render_window.draw(bulletSprite_);
}

}  // namespace game::entity
