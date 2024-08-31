#include "gui/TextCursor.hpp"

#include <cassert>

#include "gui/Text.hpp"
#include "gui/StyleSheet.hpp"

#include <iostream>

constexpr float CURSOR_WIDTH = 2.f;

namespace gui
{

TextCursor::TextCursor(gui::Text& text)
: font_{nullptr}
, text_{text}
, text_length_{}
, cursor_index_{}
, enabled_{false}
{
    auto style = BasicStyleSheetFactory::instance();
    cursor_.setFillColor(sf::Color::Black);
    cursor_.setOutlineColor(sf::Color::Black);
    cursor_.setSize(sf::Vector2f{CURSOR_WIDTH, (float)style.getFontSize()+5.f});
}

void TextCursor::setCharacterSize(uint32_t characterSize)
{
    character_size_ = characterSize;
}

void TextCursor::setFont(const sf::Font* font)
{
    font_ = font;
}

void TextCursor::update()
{
    auto cursor_position = sf::Vector2f{0.f, 0.f};

    assert(character_size_ > 0 && "Character size must be > 0");
    assert(font_ != nullptr && "Font* needs to be set");

    const auto fieldText = text_.getText();
    text_length_ = fieldText.length();

    if (cursor_index_ > text_length_) cursor_index_ = text_length_;

    for (uint32_t index = 0; index < cursor_index_; ++index)
    {
        cursor_position.x += getGlyphOffset(fieldText, index);
    }

    cursor_.setPosition(cursor_position);
    text_.updateTexture();
}

sf::Vector2f TextCursor::getPosition()
{
    return cursor_.getPosition();
}

void TextCursor::render(sf::RenderTexture& renderTexture)
{
    if (enabled_)
    {
        renderTexture.draw(cursor_);
    }
}

// TODO: consider adding update call to move methods
void TextCursor::moveLeft()
{
    if (cursor_index_ > 0) --cursor_index_; 
}

void TextCursor::moveRight()
{
    cursor_index_++;
}

float TextCursor::getGlyphOffset(const std::string& string, const size_t index)
{

    float kering_offset{};

    if(index > 0)
    {
        // On some font rendering systems kering make characters closer to each other
        // This need to be accomodated in cursor position calculations 
        kering_offset =  font_->getKerning(string[index-1],string[index], character_size_);
    }
    // FIXME: do something with false and 0.f params later
    return font_->getGlyph(string[index], character_size_, false, 0.f).advance + kering_offset;
}

void TextCursor::moveTo(float mouse_x)
{
    auto fieldText = text_.getText();
    float offset{text_.getGlobalPosition().x + text_.getOffset().x};
    size_t foundIndex{fieldText.length()};

    for (uint32_t index = 0; index < fieldText.length(); ++index)
    {
        float glyph_width = getGlyphOffset(fieldText, index);

        if (offset+glyph_width >= mouse_x)
        {
            if(offset + (glyph_width / 2.f) >= mouse_x)
            {
                foundIndex = index;  // cursor clicked on left side of a glyph (so before glyph)
            }
            else
            {
                foundIndex = index + 1; // cursor clicked on right side of a glyph (so after glyph)
            }
            break;
        }

        offset += glyph_width;
    }
    cursor_index_ = foundIndex;
}
 
uint32_t TextCursor::getIndex() const
{
    return cursor_index_;
}

void TextCursor::setIndex(const uint32_t index)
{
    cursor_index_ = index;
}

void TextCursor::disable()
{
    enabled_ = false;
}

void TextCursor::enable()
{
    enabled_ = true;
}

}  // namespace gui
