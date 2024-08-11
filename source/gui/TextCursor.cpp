#include "gui/TextCursor.hpp"

#include <cassert>

#include "gui/Text.hpp"
#include "gui/StyleSheet.hpp"

constexpr float CURSOR_WIDTH = 2.f;

namespace gui
{

TextCursor::TextCursor()
: font_{nullptr}
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

void TextCursor::update(const gui::Text& text)
{
    auto cursor_position = text.getGlobalPosition();

    assert(character_size_ > 0 && "Character size must be > 0");
    assert(font_ != nullptr && "Font* needs to be set");

    const auto fieldText = text.getText();
    text_length_ = fieldText.length();

    if (cursor_index_ > text_length_) cursor_index_ = text_length_;

    for (uint32_t index = 0; index < cursor_index_; ++index)
    {
        // FIXME: do something with false and 0.f params later
        cursor_position.x +=font_->getGlyph(fieldText[index], character_size_, false, 0.f).advance;
    }

    auto text_x_offset = text.getOffset().x;
    cursor_position.x+= text_x_offset;
    cursor_.setPosition(cursor_position);
}

void TextCursor::render(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(cursor_);
}

void TextCursor::moveLeft()
{
    if (cursor_index_ > 0) --cursor_index_; 
}

void TextCursor::moveRight()
{
    cursor_index_++;
}

void TextCursor::moveTo(const gui::Text& text, float mouse_x)
{
    auto fieldText = text.getText();
    float offset{text.getGlobalPosition().x + text.getOffset().x};
    uint32_t foundIndex{0};

    for (uint32_t index = 0; index < fieldText.length(); ++index)
    {
        // FIXME: do something with false and 0.f params later
        float glyph_width =font_->getGlyph(fieldText[index], character_size_, false, 0.f).advance;

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


}  // namespace gui