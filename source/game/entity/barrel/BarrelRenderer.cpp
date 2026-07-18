#include "game/entity/barrel/BarrelRenderer.hpp"
#include "game/entity/barrel/Barrel.hpp"

namespace game::entity
{
BarrelRenderer::BarrelRenderer(Barrel* barrel, sf::Texture& barrelBody)
: barrel_(barrel)
{
    barrelSprite_.setTexture(barrelBody);
    sf::Vector2u texture_body_size = barrelBody.getSize();
    auto middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    barrelSprite_.setOrigin(middle_point);     
}

void BarrelRenderer::draw(sf::RenderWindow& render_window)
{
    auto& barrelTransform = barrel_->transform();

    barrelSprite_.setRotation(barrelTransform.rotation());
    barrelSprite_.setColor(sf::Color(10, 10, 10, 127));
    barrelSprite_.setPosition(barrelTransform.position().x + 4, barrelTransform.position().y + 4);
   render_window.draw(barrelSprite_);

    barrelSprite_.setColor(sf::Color(255, 255, 255, 255));
    barrelSprite_.setPosition(barrelTransform.position().x, barrelTransform.position().y);
   render_window.draw(barrelSprite_);
}

}  // namespace game::entity
