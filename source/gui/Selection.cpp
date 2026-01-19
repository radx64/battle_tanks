#include "gui/Selection.hpp"

#include <cassert>
#include <algorithm>

#include "gui/Text.hpp"
#include "gui/FontHeightCache.hpp"

#include "gui/Alignment.hpp"

constexpr float CURSOR_EXTRA_HEIGHT = 6.f;

namespace gui
{

Selection::Selection(gui::Text& text)
: text_{text}
, selectionStartIndex_{}
, selectionEndIndex_{}
, selectionStartPosition_{}
, selectionEndPosition_{}
{
    //TODO move this color selection to stylesheet
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
    selectionEndIndex_ = index;
    selectionEndPosition_ = position;
    update();
}

void Selection::update()
{
    selectionRectangles_.clear();

    const std::string& fieldText = text_.getText();
    const sf::Font* font = text_.getFont();
    
    if (!font || isEmpty())
    {
        return;
    }

    // Determine actual start and end
    size_t actualStartIdx = startsAt();
    size_t actualEndIdx = endsAt();

    // Get line information
    size_t startLineNum = getLineFromIndex(fieldText, actualStartIdx);
    size_t endLineNum = getLineFromIndex(fieldText, actualEndIdx);
    
    uint32_t charSize = text_.getCharacterSize();
    float lineHeight = getFontLineSpacing(*font, charSize);
    
    // Single line selection
    if (startLineNum == endLineNum)
    {
        sf::Vector2f startPos = (selectionStartIndex_ < selectionEndIndex_) ? 
                                selectionStartPosition_ : selectionEndPosition_;
        sf::Vector2f endPos = (selectionStartIndex_ < selectionEndIndex_) ? 
                              selectionEndPosition_ : selectionStartPosition_;
        
        sf::RectangleShape rect;
        rect.setFillColor(sf::Color(100, 100, 230, 127));
        rect.setPosition(startPos);
        rect.setSize(sf::Vector2f(endPos.x - startPos.x, text_.getTextSingleLineHeight() + CURSOR_EXTRA_HEIGHT));
        selectionRectangles_.push_back(rect);
    }
    else
    {
        // Multi-line selection: create rectangles for each affected line
        
        // Determine which position is start and which is end
        sf::Vector2f startPos = (selectionStartIndex_ < selectionEndIndex_) ? 
                                selectionStartPosition_ : selectionEndPosition_;
        
        // Calculate the X position where the first line begins
        size_t firstLineStartIdx = getLineStartIndex(fieldText, startLineNum);
        float firstLineBeginX = startPos.x - getLineXWidth(fieldText, firstLineStartIdx, actualStartIdx);
        
        // First line: from selection start to end of line
        {
            size_t lineEndIdx = getLineEndIndex(fieldText, startLineNum);
            
            // Calculate width from start to end of line
            float widthToEndOfLine = getLineXWidth(fieldText, actualStartIdx, lineEndIdx);
            
            sf::RectangleShape rect;
            rect.setFillColor(sf::Color(100, 100, 230, 127));
            rect.setPosition(startPos);
            rect.setSize(sf::Vector2f(widthToEndOfLine, text_.getTextSingleLineHeight() + CURSOR_EXTRA_HEIGHT));
            selectionRectangles_.push_back(rect);
        }
        
        // Middle lines: full width for each complete line
        for (size_t lineNum = startLineNum + 1; lineNum < endLineNum; ++lineNum)
        {
            size_t lineStartIdx = getLineStartIndex(fieldText, lineNum);
            size_t lineEndIdx = getLineEndIndex(fieldText, lineNum);
            
            float lineWidth = getLineXWidth(fieldText, lineStartIdx, lineEndIdx);
            float lineYPos = startPos.y + ((lineNum - startLineNum) * lineHeight);
            
            sf::RectangleShape rect;
            rect.setFillColor(sf::Color(100, 100, 230, 127));
            rect.setPosition(firstLineBeginX, lineYPos);
            rect.setSize(sf::Vector2f(lineWidth, text_.getTextSingleLineHeight() + CURSOR_EXTRA_HEIGHT));
            selectionRectangles_.push_back(rect);
        }
        
        // Last line: from start of line to selection end
        {
            size_t lineStartIdx = getLineStartIndex(fieldText, endLineNum);
            float selectedWidth = getLineXWidth(fieldText, lineStartIdx, actualEndIdx);
            float lineYPos = startPos.y + ((endLineNum - startLineNum) * lineHeight);
            
            sf::RectangleShape rect;
            rect.setFillColor(sf::Color(100, 100, 230, 127));
            rect.setPosition(firstLineBeginX, lineYPos);
            rect.setSize(sf::Vector2f(selectedWidth, text_.getTextSingleLineHeight() + CURSOR_EXTRA_HEIGHT));
            selectionRectangles_.push_back(rect);
        }
    }

    text_.updateTexture();
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

}  // namespace gui
