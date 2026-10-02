#include "engine/gfx/Font.hpp"

#include "FontImpl.hpp"

namespace engine::gfx
{

Font::Font()
    : impl_(std::make_unique<Impl>())
{
}

Font::~Font() = default;

Font::Font(Font&&) noexcept = default;

Font& Font::operator=(Font&&) noexcept = default;

bool Font::loadFromFile(const std::string& filename)
{
    return impl_->font_.loadFromFile(filename);
}

float Font::getLineSpacing(const unsigned int characterSize) const
{
    return impl_->font_.getLineSpacing(characterSize);
}

float Font::getKerning(const std::uint32_t first, const std::uint32_t second, const unsigned int characterSize) const
{
    return impl_->font_.getKerning(first, second, characterSize);
}

float Font::getGlyphAdvance(const std::uint32_t codepoint, const unsigned int characterSize) const
{
    return impl_->font_.getGlyph(codepoint, characterSize, false).advance;
}

}  // namespace engine::gfx
