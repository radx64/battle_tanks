#include "gui/Text.hpp"

#include <cassert>

#include "gui/FontHeightCache.hpp"
#include "gui/TextDisplayModifier.hpp"

#include <gui/Alignment.hpp>

namespace gui
{

Text::Text()
: offset_{0.f, 0.f}
, sprite_(texture_.getTexture())
{
    // Allocate texture size once per text object to avoid costly resizing operations.
    // Consider adjusting the size dynamically only if absolutely necessary.
    // Or make some texture manager that will handle this and share one texture between multiple text objects.
    // TODO: decide on default size later and behaviour later
    texture_.create(2048,128);
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite_, states);
}

void Text::setOffset(const sf::Vector2f& offset)
{
    offset_ = offset;
    updateSprite();
}

const sf::Vector2f& Text::getOffset() const
{
    return offset_;
}

void Text::setGlobalPosition(const sf::Vector2f& position)
{
    globalPosition_ = position;
    updateSprite();
}

void Text::setSize(const sf::Vector2f& size)
{
    size_ = size;
}

sf::FloatRect Text::getLocalBounds() const
{
    auto bounds = text_.getLocalBounds();
    // This assures that text text alignment is ignored when sizing 
    // or aligning component from outside as top is used as origin point
    // of text object
    bounds.height += bounds.top; 
    return bounds;
}

void Text::setText(const std::string_view& text)
{
    text_.setString(text.data());
    updateTexture();
    updateSprite();
}

std::string Text::getText() const
{
    return text_.getString();
}

void Text::setFont(const sf::Font& font)
{
    text_.setFont(font);
}

const sf::Font* Text::getFont() const
{
    return text_.getFont();
}

void Text::setCharacterSize(uint32_t size)
{
    text_.setCharacterSize(size);
}

uint32_t Text::getCharacterSize() const
{
    return text_.getCharacterSize();
}

void Text::setOutlineColor(const sf::Color& color)
{
    text_.setOutlineColor(color);
}

void Text::setFillColor(const sf::Color& color)
{
    text_.setFillColor(color);
}

void Text::setOutlineThickness(float thickness)
{
    text_.setOutlineThickness(thickness);
}

void Text::updateTexture()
{
    //TODO: Consider doing something more sophisticated later
    assert (text_.getLocalBounds().width <= texture_.getSize().x && "Text width exceeded renderable texure width");

    texture_.clear(sf::Color::Transparent);

    texture_.draw(text_);

    for (auto* modifier : modifiers_)
    {
        modifier->render(texture_);
    }

    texture_.display();
}

void Text::updateSprite()
{
    sf::Vector2f textureStart{0.f, 0.f};
    sf::Vector2f positionOffset{0.f, 0.f};

    if (offset_.x > 0)
    {
        textureStart.x = 0;
        positionOffset.x = offset_.x;
    }
    else
    {
        textureStart.x = -offset_.x;
        positionOffset.x = 0;
    }

    if (offset_.y > 0)
    {
        textureStart.y = 0;
        positionOffset.y = offset_.y;
    }
    else
    {
        textureStart.y = -offset_.y;
        positionOffset.y = 0;
    }

    sprite_.setTextureRect(sf::IntRect(textureStart.x, textureStart.y, size_.x, size_.y));
    sprite_.setPosition(globalPosition_ + positionOffset);
}

sf::Vector2f Text::getGlobalPosition() const
{
    return globalPosition_;
}

sf::Vector2f Text::getSize() const
{
    return size_;
}

float Text::getTextWidth() const
{
    return text_.getLocalBounds().width;
}

float Text::getTextHeight() const
{
    return getFontHeight(*text_.getFont(), text_.getCharacterSize());
}

void Text::addModifier(TextDisplayModifier* modifier)
{
    modifiers_.push_back(modifier);
}

void Text::removeModifier(TextDisplayModifier* modifier)
{
    modifiers_.erase(
        std::remove(std::begin(modifiers_), std::end(modifiers_),modifier),
        std::end(modifiers_));
}

}  // namespace gui
