#include "gui/Text.hpp"

namespace gui
{

Text::Text()
: offset_{0.f, 0.f}
{
    // TODO decide on size 
    // According to docs try to not resize as this is costly operation
    // Consider either alocating it once per text with some reasonable size and check on some 
    // assert if size is not bigger to (either costlyu resize or ) make buffer bigger by default 
    texture_.create(1024,128);
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //TODO optimze thist later as there is no point contrtucting this whole thing every frame
    sf::Sprite sprite(texture_.getTexture());
    sprite.setTextureRect(sf::IntRect(0, 0, size_.x, size_.y));
    sprite.setPosition(global_position_);
    target.draw(sprite, states);
}

void Text::setOffset(const sf::Vector2f& offset)
{
    offset_ = offset;
    updateTexture();
}

void Text::setGlobalPosition(const sf::Vector2f& position)
{
    global_position_ = position;
}

void Text::setSize(const sf::Vector2f& size)
{
    size_ = size;
}

void Text::setText(const std::string_view& text)
{
    buffer_ = text.data();
    text_.setString(buffer_);
    updateTexture();
}

std::string Text::getText() const
{
    return buffer_;
}

void Text::setFont(const sf::Font& font)
{
    text_.setFont(font);
}

void Text::setCharacterSize(uint32_t size)
{
    text_.setCharacterSize(size);
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
    texture_.clear(sf::Color::Transparent);
    text_.setPosition(offset_);
    texture_.draw(text_);
    texture_.display();
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