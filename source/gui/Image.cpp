#include "gui/Image.hpp"

namespace gui
{

namespace 
{

sf::Vector2f getScalingFactor(const sf::Vector2u& textureSize, const sf::Vector2f& objectSize)
{
    return {objectSize.x / textureSize.x, objectSize.y / textureSize.y};
}

}  // namespace

std::unique_ptr<Image> Image::create(const sf::Texture& texture)
{
    return std::unique_ptr<Image>(new Image(texture));
}

void Image::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(sprite_);
}
void Image::setTexture(const sf::Texture& texture)
{
    sprite_.setTexture(texture);
    sprite_.setPosition(getGlobalPosition());
    sprite_.setScale(getSize().x / texture.getSize().x, getSize().y / texture.getSize().y);
}

Image::Image(const sf::Texture& texture)
: texture_(texture)
{
    setTexture(texture);
}

void Image::onPositionChange()
{
    sprite_.setPosition(getGlobalPosition());
}

void Image::onSizeChange()
{
    sprite_.setScale(getScalingFactor(texture_.getSize(), getSize()));
}

}  // namespace gui
