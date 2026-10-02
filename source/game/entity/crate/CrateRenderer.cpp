#include "game/entity/crate/CrateRenderer.hpp"
#include "game/entity/crate/Crate.hpp"

namespace game::entity
{
CrateRenderer::CrateRenderer(Crate* crate, engine::gfx::Texture& crateBody)
: crate_(crate)
{
    crateSprite_.setTexture(crateBody);
    engine::Vec2u texture_body_size = crateBody.getSize();
    auto middle_point = engine::Vec2f(texture_body_size.x / 2, texture_body_size.y / 2);
    crateSprite_.setOrigin(middle_point);
}

void CrateRenderer::draw(engine::gfx::RenderTarget& render_window)
{
    auto& crateTransform = crate_->transform();
    crateSprite_.setRotation(crateTransform.rotation());
    crateSprite_.setColor(engine::gfx::Color(10, 10, 10, 127));
    crateSprite_.setPosition(crateTransform.position().x + 4, crateTransform.position().y + 4);
   render_window.draw(crateSprite_);

    crateSprite_.setColor(engine::gfx::Color(255, 255, 255, 255));
    crateSprite_.setPosition(crateTransform.position().x, crateTransform.position().y);
   render_window.draw(crateSprite_);
}

}  // namespace game::entity
