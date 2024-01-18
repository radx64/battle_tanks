#include "game/entity/bullet/BulletRenderer.hpp"
#include "game/entity/bullet/Bullet.hpp"

namespace game::entity
{

constexpr float BULLET_SPRITE_ROTATION_OFFSET = 90.f;

BulletRenderer::BulletRenderer(Bullet* bullet, sf::Texture& bulletBody)
: bullet_(bullet)
{
    bulletSprite_.setTexture(bulletBody);
    sf::Vector2u texture_body_size = bulletBody.getSize();
    auto middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    bulletSprite_.setOrigin(middle_point);     
}

void BulletRenderer::draw(sf::RenderWindow& renderWindow)
{
    auto bulletRigidBody = bullet_->getRigidBody();

    bulletSprite_.setRotation(bulletRigidBody.rotation_ + BULLET_SPRITE_ROTATION_OFFSET);
    bulletSprite_.setColor(sf::Color(10, 10, 10, 127));
    bulletSprite_.setPosition(bulletRigidBody.x_ + 4, bulletRigidBody.y_+ 4);
    renderWindow.draw(bulletSprite_);

    bulletSprite_.setColor(sf::Color(255, 255, 255, 255));
    bulletSprite_.setPosition(bulletRigidBody.x_, bulletRigidBody.y_);
    renderWindow.draw(bulletSprite_);
}

}  // namespace game::entity