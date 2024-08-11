#include "gui/Text.hpp"

#include <cassert>

namespace gui
{

Text::Text()
: offset_{0.f, 0.f}
, sprite_(texture_.getTexture())
{
    // TODO decide on size 
    // According to docs try to not resize as this is costly operation
    // Consider either alocating it once per text with some reasonable size
    // or dynamic resize if really really needed
    texture_.create(2048,128);
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite_, states);
}

void Text::setOffset(const sf::Vector2f& offset)
{
    offset_ = offset;
    updateTexture();
}

const sf::Vector2f& Text::getOffset() const
{
    return offset_;
}

void Text::setGlobalPosition(const sf::Vector2f& position)
{
    global_position_ = position;
    updateSprite();
}

void Text::setSize(const sf::Vector2f& size)
{
    size_ = size;
}

void Text::setText(const std::string_view& text)
{
    text_.setString(text.data());
    updateTexture();
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
    text_.setPosition(offset_);
    texture_.draw(text_);
    texture_.display();
}

void Text::updateSprite()
{
    sprite_.setTextureRect(sf::IntRect(0, 0, size_.x, size_.y));
    sprite_.setPosition(global_position_);
}

sf::Vector2f Text::getGlobalPosition() const
{
    return global_position_;
}

sf::Vector2f Text::getSize() const
{
    return size_;
}

float Text::getTextWidth() const
{
    return text_.getLocalBounds().width;
}

}  // namespace gui