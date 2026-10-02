#include "engine/gfx/Text.hpp"

#include "FontImpl.hpp"
#include "TextImpl.hpp"

#include "backends/sfml/ToSf.hpp"

namespace engine::gfx
{

Text::Text()
    : impl_(std::make_unique<Impl>())
{
}

Text::~Text() = default;

Text::Text(Text&&) noexcept = default;

Text& Text::operator=(Text&&) noexcept = default;

void Text::setFont(const Font& font)
{
    impl_->font_ = &font;
    impl_->text_.setFont(font.impl_->font_);
}

const Font* Text::getFont() const
{
    return impl_->font_;
}

void Text::setCharacterSize(const std::uint32_t size)
{
    impl_->text_.setCharacterSize(size);
}

std::uint32_t Text::getCharacterSize() const
{
    return impl_->text_.getCharacterSize();
}

void Text::setString(const std::string_view text)
{
    impl_->text_.setString(std::string{text});
}

std::string Text::getString() const
{
    return impl_->text_.getString().toAnsiString();
}

void Text::setStyle(const std::uint32_t style)
{
    impl_->text_.setStyle(style);
}

void Text::setFillColor(const Color& color)
{
    impl_->text_.setFillColor(engine::toSf(color));
}

Color Text::getFillColor() const
{
    const auto color = impl_->text_.getFillColor();
    return Color{color.r, color.g, color.b, color.a};
}

void Text::setOutlineColor(const Color& color)
{
    impl_->text_.setOutlineColor(engine::toSf(color));
}

Color Text::getOutlineColor() const
{
    const auto color = impl_->text_.getOutlineColor();
    return Color{color.r, color.g, color.b, color.a};
}

void Text::setOutlineThickness(const float thickness)
{
    impl_->text_.setOutlineThickness(thickness);
}

void Text::setOrigin(const float x, const float y)
{
    impl_->text_.setOrigin(x, y);
}

void Text::setPosition(const engine::Vec2f& position)
{
    impl_->text_.setPosition(position.x, position.y);
}

void Text::setPosition(const float x, const float y)
{
    impl_->text_.setPosition(x, y);
}

engine::FloatRect Text::getLocalBounds() const
{
    const auto bounds = impl_->text_.getLocalBounds();
    return engine::FloatRect{bounds.left, bounds.top, bounds.width, bounds.height};
}

}  // namespace engine::gfx
