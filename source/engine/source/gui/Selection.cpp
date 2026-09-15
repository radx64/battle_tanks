#include "engine/gui/Selection.hpp"

#include <cassert>
#include <algorithm>
#include <utility>

#include "engine/gui/Text.hpp"
#include "engine/gui/FontHeightCache.hpp"

#include "engine/gui/Alignment.hpp"

constexpr float CURSOR_EXTRA_HEIGHT = 6.f;

namespace engine::gui
{

Selection::Selection(engine::gui::Text& text, const sf::Color& selectionColor)
: text_{text}
, selectionStartIndex_{}
, selectionEndIndex_{}
, selectionStartPosition_{}
, selectionEndPosition_{}
, selectionColor_{selectionColor}
{
}

bool Selection::isEmpty() const
{
    return selectionStartIndex_ == selectionEndIndex_;
}

void Selection::start(const uint32_t index, const sf::Vector2f& position)
{
    selectionStartIndex_ = index;
    selectionStartPosition_ = position;

    selectionEndIndex_ = index;
    selectionEndPosition_ = position;
    update();
}

size_t Selection::startsAt() const
{
    return selectionStartIndex_ < selectionEndIndex_ ? selectionStartIndex_ : selectionEndIndex_;
}

size_t Selection::endsAt() const
{
    return selectionStartIndex_ < selectionEndIndex_ ? selectionEndIndex_ : selectionStartIndex_;
}

size_t Selection::length() const
{
    return endsAt() - startsAt();
}

void Selection::clear()
{
    selectionEndIndex_ = selectionStartIndex_;
    selectionEndPosition_ = selectionStartPosition_;
    update();
}

void Selection::to(const uint32_t index, const sf::Vector2f& position)
{
    if (selectionEndIndex_ == index)
    {
        return;
    }

    selectionEndIndex_ = index;
    selectionEndPosition_ = position;
    update();
}

void Selection::update()
{
    selectionRectangles_.clear();
    text_.invalidateTexture();

    const std::string& fieldText = text_.getText();
    const sf::Font* font = text_.getFont();
    
    if (!font || isEmpty())
    {
        return;
    }

    const bool selectionForward = selectionStartIndex_ < selectionEndIndex_;
    const size_t actualStartIdx = startsAt();
    const size_t actualEndIdx = endsAt();
    const sf::Vector2f startPos = selectionForward ? selectionStartPosition_ : selectionEndPosition_;
    const sf::Vector2f endPos = selectionForward ? selectionEndPosition_ : selectionStartPosition_;

    const auto getLineInfo = [&fieldText](const size_t index)
    {
        size_t lineNumber = 0;
        size_t lineStartIdx = 0;

        for (size_t i = 0; i < index && i < fieldText.length(); ++i)
        {
            if (fieldText[i] == '\n')
            {
                ++lineNumber;
                lineStartIdx = i + 1;
            }
        }

        return std::pair{lineNumber, lineStartIdx};
    };

    const auto [startLineNum, firstLineStartIdx] = getLineInfo(actualStartIdx);
    const auto [endLineNum, ignoredEndLineStartIdx] = getLineInfo(actualEndIdx);
    (void)ignoredEndLineStartIdx;

    const uint32_t charSize = text_.getCharacterSize();
    const float lineHeight = getFontLineSpacing(*font, charSize);

    const float lineBeginX = startPos.x - getLineXWidth(fieldText, firstLineStartIdx, actualStartIdx);

    size_t fullLineStartIdx = firstLineStartIdx;
    for (size_t lineNum = startLineNum; lineNum <= endLineNum; ++lineNum)
    {
        const bool isFirstLine = lineNum == startLineNum;
        const bool isLastLine = lineNum == endLineNum;

        const size_t lineStartIdx = isFirstLine ? actualStartIdx : fullLineStartIdx;
        const size_t fullLineEndIdx = fieldText.find('\n', fullLineStartIdx);
        const size_t lineEndIdx = isLastLine ? actualEndIdx : fullLineEndIdx;

        const float lineYPos = startPos.y + ((lineNum - startLineNum) * lineHeight);
        const float rectX = isFirstLine ? startPos.x : lineBeginX;
        const float rectWidth = isFirstLine && isLastLine
            ? endPos.x - startPos.x
            : getLineXWidth(fieldText, lineStartIdx, lineEndIdx);

        sf::RectangleShape rect;
        rect.setFillColor(selectionColor_);
        rect.setPosition({rectX, lineYPos});
        rect.setSize({rectWidth, lineHeight});
        selectionRectangles_.push_back(rect);

        fullLineStartIdx = fullLineEndIdx + 1;
    }
}

size_t Selection::getLineFromIndex(const std::string& text, size_t index) const
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

void Selection::render(sf::RenderTexture& renderTexture)
{
    for (size_t i = 0; i < selectionRectangles_.size(); ++i)
    {
        auto& rect = selectionRectangles_[i];
        renderTexture.draw(rect);
    }
}

size_t Selection::getColumnFromIndex(const std::string& text, size_t index) const
{
    size_t lineStartIdx = getLineStartIndex(text, getLineFromIndex(text, index));
    return index - lineStartIdx;
}

size_t Selection::getLineStartIndex(const std::string& text, size_t lineNumber) const
{
    size_t currentLine = 0;
    size_t pos = 0;
    
    while (currentLine < lineNumber && pos < text.length())
    {
        if (text[pos] == '\n')
        {
            currentLine++;
            pos++;
        }
        else
        {
            pos++;
        }
    }
    
    return pos;
}

size_t Selection::getLineEndIndex(const std::string& text, size_t lineNumber) const
{
    size_t lineStartIdx = getLineStartIndex(text, lineNumber);
    size_t endIdx = lineStartIdx;
    
    while (endIdx < text.length() && text[endIdx] != '\n')
    {
        endIdx++;
    }
    
    return endIdx;
}

float Selection::getLineXWidth(const std::string& text, size_t lineStartIdx, size_t lineEndIdx) const
{
    const sf::Font* font = text_.getFont();
    if (!font) return 0.f;
    
    uint32_t charSize = text_.getCharacterSize();
    float width = 0.f;
    
    for (size_t i = lineStartIdx; i < lineEndIdx && i < text.length(); ++i)
    {
        float keringOffset = 0.f;
        if (i > lineStartIdx)
        {
            keringOffset = font->getKerning(text[i-1], text[i], charSize);
        }
        width += font->getGlyph(text[i], charSize, false, 0.f).advance + keringOffset;
    }
    
    return width;
}

}  // namespace engine::gui
