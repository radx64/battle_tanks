#include "gui/TextCursor.hpp"

#include <cassert>

#include "gui/Text.hpp"
#include "gui/StyleSheet.hpp"

constexpr float CURSOR_WIDTH = 2.f;

namespace gui
{

TextCursor::TextCursor(gui::Text& text)
: font_{nullptr}
, text_{text}
, textLength_{}
, cursorIndex_{}
, enabled_{false}
{
    auto style = BasicStyleSheetFactory::instance();
    cursor_.setFillColor(sf::Color::Black);
    cursor_.setOutlineColor(sf::Color::Black);
    cursor_.setSize(sf::Vector2f{CURSOR_WIDTH, (float)style.getFontSize()+5.f});
}

void TextCursor::setCharacterSize(uint32_t characterSize)
{
    characterSize_ = characterSize;
}

void TextCursor::setFont(const sf::Font* font)
{
    font_ = font;
}

void TextCursor::update()
{
    auto cursorPosition = sf::Vector2f{0.f, 0.f};

    assert(characterSize_ > 0 && "Character size must be > 0");
    assert(font_ != nullptr && "Font* needs to be set");

    const auto fieldText = text_.getText();
    textLength_ = fieldText.length();

    if (cursorIndex_ > textLength_) cursorIndex_ = textLength_;

    for (uint32_t index = 0; index < cursorIndex_; ++index)
    {
        cursorPosition.x += getGlyphOffset(fieldText, index);
    }

    cursor_.setPosition(cursorPosition);
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

void TextCursor::moveLeft(const bool moveWholeWord)
{
    if (cursorIndex_ == 0)
    {
        return;
    }
    if (not moveWholeWord)
    {
        --cursorIndex_;
    }
    else
    {
        const auto& text = text_.getText();

        auto characterBeforeCursor = cursorIndex_ - 1;
        if (characterBeforeCursor <= text.length() && text.at(characterBeforeCursor) == ' ')
        {
            auto cursorPositionWithSkippedSpaces = text.find_last_not_of(' ', characterBeforeCursor);
            if (cursorPositionWithSkippedSpaces == std::string::npos)
            {
                characterBeforeCursor = 0; 
            }
            else
            {
                characterBeforeCursor = cursorPositionWithSkippedSpaces - 1;
            }
        }

        auto lastSpace = text.find_last_of(' ', characterBeforeCursor);
        if (lastSpace != std::string::npos)
        {
            cursorIndex_ = lastSpace + 1;
        }
        else
        {
            cursorIndex_ = 0;
        }
    }
    update();
}

void TextCursor::moveRight(const bool moveWholeWord)
{    
    if (not moveWholeWord)
    {
        cursorIndex_++;
    }
    else
    {
        const auto& text = text_.getText();

        auto characterAfterCursor = cursorIndex_ + 1;
        if (characterAfterCursor < text.length() && text.at(characterAfterCursor) == ' ')
        {
            auto cursorPositionWithSkippedSpaces = text.find_first_not_of(' ', characterAfterCursor);
            if (cursorPositionWithSkippedSpaces == std::string::npos)
            {
                characterAfterCursor = text.length(); 
            }
            else
            {
                characterAfterCursor = cursorPositionWithSkippedSpaces + 1;
            }
        }

        auto firstSpace = text.find_first_of(' ', characterAfterCursor);
        if (firstSpace != std::string::npos)
        {
            cursorIndex_ = firstSpace;
        }
        else
        {
            cursorIndex_ = text.length();
        }
    }
    update();
}

float TextCursor::getGlyphOffset(const std::string& string, const size_t index)
{
    float keringOffset{};

    if(index > 0)
    {
        // On some font rendering systems kering make characters closer to each other
        // This need to be accomodated in cursor position calculations
        keringOffset =  font_->getKerning(string[index-1],string[index], characterSize_);
    }
    // FIXME: do something with false and 0.f params later
    return font_->getGlyph(string[index], characterSize_, false, 0.f).advance + keringOffset;
}

void TextCursor::moveTo(float mouseX)
{
    auto fieldText = text_.getText();
    float offset{text_.getGlobalPosition().x + text_.getOffset().x};
    size_t foundIndex{fieldText.length()};

    for (uint32_t index = 0; index < fieldText.length(); ++index)
    {
        float glyphWidth = getGlyphOffset(fieldText, index);

        if (offset+glyphWidth >= mouseX)
        {
            if(offset + (glyphWidth / 2.f) >= mouseX)
            {
                foundIndex = index;  // cursor clicked on left side of a glyph (so before glyph)
            }
            else
            {
                foundIndex = index + 1; // cursor clicked on right side of a glyph (so after glyph)
            }
            break;
        }

        offset += glyphWidth;
    }
    cursorIndex_ = foundIndex;
    update();
}

uint32_t TextCursor::getIndex() const
{
    return cursorIndex_;
}

void TextCursor::setIndex(const uint32_t index)
{
    cursorIndex_ = index;
    update();
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
