#include "gui/TextCursor.hpp"

#include <cassert>

#include "gui/Text.hpp"
#include "gui/StyleSheet.hpp"

constexpr float CURSOR_WIDTH = 2.f;

namespace gui
{

TextCursor::TextCursor(const gui::Text& text)
: font_{nullptr}
, text_{text}
, text_length_{}
, cursor_index_{}
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
    auto cursor_position = text_.getGlobalPosition();

    assert(character_size_ > 0 && "Character size must be > 0");
    assert(font_ != nullptr && "Font* needs to be set");

    const auto fieldText = text_.getText();
    text_length_ = fieldText.length();

    if (cursor_index_ > text_length_) cursor_index_ = text_length_;

    for (uint32_t index = 0; index < cursor_index_; ++index)
    {
        cursor_position.x += getGlyphSizeAt(fieldText, index);
    }

    auto text_x_offset = text_.getOffset().x;
    cursor_position.x+= text_x_offset;
    cursor_.setPosition(cursor_position);
}

sf::Vector2f TextCursor::getGlobalPosition()
{
    return cursor_.getPosition();
}

void TextCursor::render(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(cursor_);
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

float TextCursor::getGlyphSizeAt(const std::string& string, const size_t index)
{
    // FIXME: do something with false and 0.f params later
    return font_->getGlyph(string[index], character_size_, false, 0.f).advance;
}

void TextCursor::moveTo(float mouse_x)
{
    auto fieldText = text_.getText();
    float offset{text_.getGlobalPosition().x + text_.getOffset().x};
    size_t foundIndex{fieldText.length()};

    for (uint32_t index = 0; index < fieldText.length(); ++index)
    {
        float glyph_width = getGlyphSizeAt(fieldText, index);

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


}  // namespace gui