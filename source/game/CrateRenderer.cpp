#include "game/CrateRenderer.hpp"
#include "game/Crate.hpp"

namespace game
{
CrateRenderer::CrateRenderer(Crate* crate, sf::Texture& crateBody)
: crate_(crate)
{
    crateSprite_.setTexture(crateBody);
    sf::Vector2u texture_body_size = crateBody.getSize();
    auto middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    crateSprite_.setOrigin(middle_point);
}

void CrateRenderer::draw(sf::RenderWindow& renderWindow)
{
    auto crateRigidBody = crate_->getRigidBody();
    crateSprite_.setRotation(crateRigidBody.rotation_);
    crateSprite_.setColor(sf::Color(10, 10, 10, 127));
    crateSprite_.setPosition(crateRigidBody.x_ + 4, crateRigidBody.y_+ 4);
    renderWindow.draw(crateSprite_);

    crateSprite_.setColor(sf::Color(255, 255, 255, 255));
    crateSprite_.setPosition(crateRigidBody.x_, crateRigidBody.y_);
    renderWindow.draw(crateSprite_);
}

}  // namespace game