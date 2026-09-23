#include "engine/gfx/Texture.hpp"

#include "TextureImpl.hpp"

namespace engine::gfx
{

Texture::Texture()
: impl_(std::make_shared<Impl>())
{
}

Texture::~Texture() = default;

Texture::Texture(std::shared_ptr<Impl> impl)
: impl_(std::move(impl))
{
}

bool Texture::loadFromFile(const std::string& filename)
{
    return impl_->texture_->loadFromFile(filename);
}

engine::Vec2u Texture::getSize() const
{
    const auto impl_size = impl_->texture_->getSize();
    return engine::Vec2u{impl_size.x, impl_size.y};
}

} // namespace engine::gfx
