#include "engine/gfx/RenderTexture.hpp"

#include "RenderTextureImpl.hpp"
#include "TextureImpl.hpp"

namespace engine::gfx
{

RenderTexture::RenderTexture()
: impl_(std::make_unique<Impl>())
{
}

RenderTexture::~RenderTexture() = default;

engine::gfx::Texture RenderTexture::getTexture() const
{
	auto textureImpl = std::make_shared<engine::gfx::Texture::Impl>();
	textureImpl->texture_ = std::shared_ptr<sf::Texture>{
		impl_->render_texture_,
		const_cast<sf::Texture*>(&impl_->render_texture_->getTexture())};

	return engine::gfx::Texture{std::move(textureImpl)};
}

}  // namespace engine::gfx
