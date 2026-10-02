#include "engine/gfx/RenderTexture.hpp"

#include "RenderTextureImpl.hpp"
#include "SfmlRenderTarget.hpp"
#include "TextureImpl.hpp"

namespace engine::gfx
{

namespace
{
engine::backends::sfml::SfmlRenderTarget targetOf(RenderTexture::Impl& impl)
{
    return engine::backends::sfml::SfmlRenderTarget{*impl.render_texture_};
}
}  // namespace

RenderTexture::RenderTexture()
    : impl_(std::make_unique<Impl>())
{
}

RenderTexture::~RenderTexture() = default;

RenderTexture::RenderTexture(RenderTexture&&) noexcept = default;

RenderTexture& RenderTexture::operator=(RenderTexture&&) noexcept = default;

bool RenderTexture::create(const unsigned int width, const unsigned int height)
{
    return impl_->render_texture_->create(width, height);
}

void RenderTexture::display()
{
    impl_->render_texture_->display();
}

engine::Vec2u RenderTexture::getSize() const
{
    const auto size = impl_->render_texture_->getSize();
    return engine::Vec2u{size.x, size.y};
}

engine::gfx::Texture RenderTexture::getTexture() const
{
    auto textureImpl = std::make_shared<engine::gfx::Texture::Impl>();
    textureImpl->texture_ = std::shared_ptr<sf::Texture>{
        impl_->render_texture_,
        const_cast<sf::Texture*>(&impl_->render_texture_->getTexture())};

    return engine::gfx::Texture{std::move(textureImpl)};
}

void RenderTexture::clear(const Color& color)
{
    targetOf(*impl_).clear(color);
}

void RenderTexture::draw(const Sprite& sprite, const RenderStates& states)
{
    targetOf(*impl_).draw(sprite, states);
}

void RenderTexture::draw(const RectangleShape& shape, const RenderStates& states)
{
    targetOf(*impl_).draw(shape, states);
}

void RenderTexture::draw(const CircleShape& shape, const RenderStates& states)
{
    targetOf(*impl_).draw(shape, states);
}

void RenderTexture::draw(const Text& text, const RenderStates& states)
{
    targetOf(*impl_).draw(text, states);
}

void RenderTexture::draw(const VertexArray& vertices, const RenderStates& states)
{
    targetOf(*impl_).draw(vertices, states);
}

void RenderTexture::drawVertices(const VertexArray& vertices, const Texture& texture, const RenderStates& states)
{
    targetOf(*impl_).drawVertices(vertices, texture, states);
}

}  // namespace engine::gfx
