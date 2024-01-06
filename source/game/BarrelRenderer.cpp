#include "game/BarrelRenderer.hpp"
#include "game/Barrel.hpp"

namespace game
{
BarrelRenderer::BarrelRenderer(Barrel* barrel, sf::Texture& barrelBody)
: barrel_(barrel)
{
    barrelSprite_.setTexture(barrelBody);
    sf::Vector2u texture_body_size = barrelBody.getSize();
    auto middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    barrelSprite_.setOrigin(middle_point);     
}

void BarrelRenderer::draw(sf::RenderWindow& renderWindow)
{
    auto barrelRigidBody = barrel_->getRigidBody();

    barrelSprite_.setRotation(barrelRigidBody.rotation_);
    barrelSprite_.setColor(sf::Color(10, 10, 10, 127));
    barrelSprite_.setPosition(barrelRigidBody.x_ + 4, barrelRigidBody.y_+ 4);
    renderWindow.draw(barrelSprite_);

    barrelSprite_.setColor(sf::Color(255, 255, 255, 255));
    barrelSprite_.setPosition(barrelRigidBody.x_, barrelRigidBody.y_);
    renderWindow.draw(barrelSprite_);
}

}  // namespace game