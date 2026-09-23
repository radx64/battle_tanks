#include "engine/gui/Image.hpp"

namespace engine::gui
{

namespace 
{

engine::Vec2f getScalingFactor(const engine::Vec2u& textureSize, const engine::Vec2f& objectSize)
{
    return {objectSize.x / textureSize.x, objectSize.y / textureSize.y};
}

}  // namespace

std::unique_ptr<Image> Image::create(const engine::gfx::Texture& texture)
{
    return std::unique_ptr<Image>(new Image(texture));
}

void Image::onRender(engine::gfx::RenderTarget& renderTarget)
{
    renderTarget.draw(sprite_);
}
void Image::setTexture(const engine::gfx::Texture& texture)
{
    sprite_.setTexture(texture);

    const auto position = getGlobalPosition();

    sprite_.setPosition(position);
    sprite_.setScale(getSize().x / texture.getSize().x, getSize().y / texture.getSize().y);
}

Image::Image(const engine::gfx::Texture& texture)
: texture_(texture)
{
    setTexture(texture);
}

void Image::onPositionChange()
{
    const auto position = getGlobalPosition();
    sprite_.setPosition(position);
}

void Image::onSizeChange()
{
    sprite_.setScale(getScalingFactor(texture_.getSize(), getSize()));
}

}  // namespace engine::gui
