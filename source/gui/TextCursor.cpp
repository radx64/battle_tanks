#include "gui/TextCursor.hpp"

#include <cassert>

#include "engine/Context.hpp"
#include "engine/TimerService.hpp"

#include "gui/FontHeightCache.hpp"
#include "gui/Text.hpp"
#include "gui/StyleSheet.hpp"

constexpr float CURSOR_WIDTH = 4.f;
constexpr float CURSOR_EXTRA_HEIGHT = 10.f;

using namespace std::literals;

namespace gui
{

TextCursor::TextCursor(gui::Text& text)
: font_{nullptr}
, text_{text}
, textLength_{}
, cursorIndex_{}
, enabled_{false}
, isCursorVisible_{false}
, blinkTimer_{500ms, [this]{animateCursor();}}
{
    cursorImage_.setFillColor(sf::Color::Black);
    cursorImage_.setOutlineColor(sf::Color::Black);
}

void TextCursor::animateCursor()
{
    if (not enabled_)
    {
        return;
    }
    isCursorVisible_ = !isCursorVisible_;
    update();
}

void TextCursor::setCharacterSize(uint32_t characterSize)
{
    characterSize_ = characterSize;
    assert(font_ != nullptr && "Font* needs to be set before setting character size");
    float actualFontHeight = getFontHeight(*font_, characterSize);
    cursorImage_.setSize(sf::Vector2f{CURSOR_WIDTH, actualFontHeight});
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

    // Handle multiline text by finding the line and column
    size_t lineNumber = getLineFromIndex(fieldText, cursorIndex_);
    size_t columnInLine = getColumnFromIndex(fieldText, cursorIndex_);
    
    // Calculate X position: sum of glyph widths on the current line up to cursor column
    size_t lineStartIdx = 0;
    for (size_t i = 0; i < lineNumber; ++i)
    {
        // Find the start of the next line (skip past the newline)
        size_t newlinePos = fieldText.find('\n', lineStartIdx);
        if (newlinePos != std::string::npos)
        {
            lineStartIdx = newlinePos + 1;
        }
        else
        {
            lineStartIdx = fieldText.length();
            break;
        }
    }
    
    // Sum glyph widths on this line up to cursor column
    for (size_t i = 0; i < columnInLine; ++i)
    {
        if (lineStartIdx + i < fieldText.length())
        {
            cursorPosition.x += getGlyphOffset(fieldText, lineStartIdx + i);
        }
    }

    // Set Y position based on line number
    float lineHeight = getLineHeight();
    cursorPosition.y = lineNumber * lineHeight;

    cursorImage_.setPosition(cursorPosition);
    text_.updateTexture();
}

size_t TextCursor::getLineFromIndex(const std::string& text, size_t index) const
{
    size_t lineNumber = 0;
    for (size_t i = 0; i < index && i < text.length(); ++i)
    {
        if (text[i] == '\n')
        {
            lineNumber++;
        }
    }
    return lineNumber;
}

size_t TextCursor::getColumnFromIndex(const std::string& text, size_t index) const
{
    // Find the start of the current line
    size_t lineStartIdx = 0;
    for (size_t i = 0; i < index && i < text.length(); ++i)
    {
        if (text[i] == '\n')
        {
            lineStartIdx = i + 1;
        }
    }
    // Column is the distance from line start to cursor
    return index - lineStartIdx;
}

float TextCursor::getLineHeight() const
{
    // Use SFML's line spacing which includes the gap between lines
    return getFontLineSpacing(*font_, characterSize_);
}

sf::Vector2f TextCursor::getPosition()
{
    return cursorImage_.getPosition();
}

void TextCursor::render(sf::RenderTexture& renderTexture)
{
    if (isCursorVisible_)
    {
        renderTexture.draw(cursorImage_);
    }
}

void TextCursor::moveLeft(const bool moveWholeWord)
{
    startBlinkAnimation();
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
    startBlinkAnimation();
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
    startBlinkAnimation();
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
    startBlinkAnimation();
    update();
}

void TextCursor::disable()
{
    if (not enabled_) return;
    enabled_ = false;
    stopBlinkAnimation();
    update();
}

void TextCursor::enable()
{
    if (enabled_) return;
    enabled_ = true;
    startBlinkAnimation();
    update();
}

void TextCursor::startBlinkAnimation()
{
    isCursorVisible_ = true;
    engine::Context::getTimerService().restart(&blinkTimer_, engine::TimerType::Repeating);
}

void TextCursor::stopBlinkAnimation()
{
    isCursorVisible_ = false;
    engine::Context::getTimerService().cancel(&blinkTimer_);
}

}  // namespace gui
