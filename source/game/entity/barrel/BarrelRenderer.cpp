#include "game/entity/barrel/BarrelRenderer.hpp"
#include "game/entity/barrel/Barrel.hpp"

namespace game::entity
{
BarrelRenderer::BarrelRenderer(Barrel* barrel, engine::gfx::Texture& barrelBody)
: barrel_(barrel)
{
    barrelSprite_.setTexture(barrelBody);
    engine::Vec2u texture_body_size = barrelBody.getSize();
    auto middle_point = engine::Vec2f(texture_body_size.x / 2, texture_body_size.y / 2);
    barrelSprite_.setOrigin(middle_point);     
}

void BarrelRenderer::draw(engine::gfx::RenderTarget& render_window)
{
    auto& barrelTransform = barrel_->transform();

    barrelSprite_.setRotation(barrelTransform.rotation());
    barrelSprite_.setColor(engine::gfx::Color(10, 10, 10, 127));
    barrelSprite_.setPosition(barrelTransform.position().x + 4, barrelTransform.position().y + 4);
    render_window.draw(barrelSprite_);

    barrelSprite_.setColor(engine::gfx::Color(255, 255, 255, 255));
    barrelSprite_.setPosition(barrelTransform.position().x, barrelTransform.position().y);
    render_window.draw(barrelSprite_);
}

}  // namespace game::entity
