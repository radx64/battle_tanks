#include "engine/gfx/Sprite.hpp"

#include "SpriteImpl.hpp"
#include "TextureImpl.hpp"
#include "backends/sfml/ToSf.hpp"

namespace engine::gfx
{

Sprite::Sprite()
: impl_(std::make_unique<Impl>())
{
}

Sprite::Sprite(const engine::gfx::Texture& texture)
: Sprite()
{
    setTexture(texture);
}

Sprite::~Sprite() = default;

Sprite::Sprite(Sprite&&) noexcept = default;

Sprite& Sprite::operator=(Sprite&&) noexcept = default;

void Sprite::setPosition(const engine::Vec2f& position)
{
    impl_->sprite_.setPosition(position.x, position.y);
}

void Sprite::setPosition(const float x, const float y)
{
    impl_->sprite_.setPosition(x, y);
}

engine::Vec2f Sprite::getPosition() const
{
    const auto position = impl_->sprite_.getPosition();
    return engine::Vec2f{position.x, position.y};
}

void Sprite::setRotation(const float rotation)
{
    impl_->sprite_.setRotation(rotation);
}

void Sprite::setOrigin(const engine::Vec2f& origin)
{
    impl_->sprite_.setOrigin(origin.x, origin.y);
}

void Sprite::setOrigin(const float x, const float y)
{
    impl_->sprite_.setOrigin(x, y);
}

void Sprite::setTexture(const Texture& texture, const bool resetRect)
{
    impl_->texture_ = texture;
    impl_->sprite_.setTexture(*impl_->texture_.impl_->texture_, resetRect);
}

void Sprite::setTextureRect(const engine::IntRect& rectangle)
{
    impl_->sprite_.setTextureRect(engine::toSf(rectangle));
}

void Sprite::setColor(const Color& color)
{
    impl_->sprite_.setColor(engine::toSf(color));
}

void Sprite::setScale(const float x, const float y)
{
    impl_->sprite_.setScale(x, y);
}

void Sprite::setScale(const engine::Vec2f& scale)
{
    impl_->sprite_.setScale(scale.x, scale.y);
}

engine::FloatRect Sprite::getLocalBounds() const
{
    const auto bounds = impl_->sprite_.getLocalBounds();

    return engine::FloatRect{bounds.left,
                             bounds.top,
                             bounds.width,
                             bounds.height};
}

engine::FloatRect Sprite::getGlobalBounds() const
{
    const auto bounds = impl_->sprite_.getGlobalBounds();

    return engine::FloatRect{bounds.left,
                             bounds.top,
                             bounds.width,
                             bounds.height};
}

}  // namespace engine::gfx
