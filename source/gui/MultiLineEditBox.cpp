#include "gui/MultiLineEditBox.hpp"

#include <fmt/format.h>

#include "gui/Clipboard.hpp"
#include "gui/Debug.hpp"
#include "gui/FontHeightCache.hpp"
#include "gui/StyleSheet.hpp"
#include "gui/TextureLibrary.hpp"

namespace 
{
const sf::Vector2f EXTRA_END_OFFSET{6.f, 4.f};
constexpr uint32_t DEFAULT_TEXT_MAX_LENGTH = 2048;
constexpr uint32_t DEFAULT_MAX_LINES = 10;
}  // namespace

namespace gui
{

std::unique_ptr<MultiLineEditBox> MultiLineEditBox::create()
{
    return std::unique_ptr<MultiLineEditBox>{new MultiLineEditBox{}};
}

MultiLineEditBox::MultiLineEditBox()
: BaseEditBox()
, maxLines_{DEFAULT_MAX_LINES}
{
    maxLength_ = DEFAULT_TEXT_MAX_LENGTH;
    rebuildLineIndices();
}

void MultiLineEditBox::updateTextVisbleArea()
{
    // For multiline text, we typically use top-left alignment
    // with vertical scrolling capability (simplified for now)
    text_.setOffset(sf::Vector2f{0.f, 0.f});
}

void MultiLineEditBox::onTextChanged()
{
    rebuildLineIndices();
}

void MultiLineEditBox::rebuildLineIndices()
{
    lineIndices_.clear();
    
    const std::string& textStr = text_.getText();
    size_t startIdx = 0;
    
    for (size_t i = 0; i <= textStr.length(); ++i)
    {
        if (i == textStr.length() || textStr[i] == '\n')
        {
            lineIndices_.push_back({startIdx, i - startIdx});
            startIdx = i + 1;
        }
    }
    
    // Ensure at least one line exists
    if (lineIndices_.empty())
    {
        lineIndices_.push_back({0, 0});
    }
}

void MultiLineEditBox::updateCursorPosition()
{
    textCursor_.update();
}

size_t MultiLineEditBox::getLineFromIndex(size_t textIndex) const
{
    for (size_t lineIdx = 0; lineIdx < lineIndices_.size(); ++lineIdx)
    {
        const auto& line = lineIndices_[lineIdx];
        if (textIndex >= line.startIndex && textIndex <= line.startIndex + line.length)
        {
            return lineIdx;
        }
    }
    return lineIndices_.size() > 0 ? lineIndices_.size() - 1 : 0;
}

size_t MultiLineEditBox::getColumnFromIndex(size_t textIndex) const
{
    size_t lineIdx = getLineFromIndex(textIndex);
    if (lineIdx < lineIndices_.size())
    {
        return textIndex - lineIndices_[lineIdx].startIndex;
    }
    return 0;
}

size_t MultiLineEditBox::getIndexFromLineColumn(size_t line, size_t column) const
{
    if (line >= lineIndices_.size())
    {
        line = lineIndices_.size() - 1;
    }
    
    const auto& lineInfo = lineIndices_[line];
    size_t maxCol = lineInfo.length;
    
    // Clamp column to line length
    if (column > maxCol)
    {
        column = maxCol;
    }
    
    return lineInfo.startIndex + column;
}

size_t MultiLineEditBox::getIndexFromScreenPosition(const sf::Vector2f& screenPos)
{
    const std::string& fieldText = text_.getText();
    const sf::Vector2f textGlobalPos = text_.getGlobalPosition();
    const sf::Vector2f textOffset = text_.getOffset();
    
    // Get font info
    const sf::Font* font = text_.getFont();
    if (!font) return 0;
    
    uint32_t charSize = text_.getCharacterSize();
    float lineHeight = getFontLineSpacing(*font, charSize);
    
    // Calculate which line was clicked
    float relativeY = screenPos.y - textGlobalPos.y;
    size_t lineIdx = static_cast<size_t>(relativeY / lineHeight);
    
    // Clamp to valid line range
    if (lineIdx >= lineIndices_.size())
    {
        lineIdx = lineIndices_.size() - 1;
    }
    
    // Get the text for the clicked line
    const LineInfo& lineInfo = lineIndices_[lineIdx];
    std::string lineText = fieldText.substr(lineInfo.startIndex, lineInfo.length);
    
    // Find column based on X position
    float xOffset = textGlobalPos.x + textOffset.x;
    size_t foundColumn = lineText.length();
    
    float currentX = xOffset;
    for (size_t i = 0; i < lineText.length(); ++i)
    {
        // Calculate glyph width using the same logic as TextCursor
        float keringOffset = 0.f;
        if (i > 0)
        {
            keringOffset = font->getKerning(lineText[i-1], lineText[i], charSize);
        }
        float glyphWidth = font->getGlyph(lineText[i], charSize, false, 0.f).advance + keringOffset;
        
        if (currentX + glyphWidth >= screenPos.x)
        {
            if (currentX + (glyphWidth / 2.f) >= screenPos.x)
            {
                foundColumn = i;  // Left side of glyph
            }
            else
            {
                foundColumn = i + 1;  // Right side of glyph
            }
            break;
        }
        currentX += glyphWidth;
    }
    
    return getIndexFromLineColumn(lineIdx, foundColumn);
}

void MultiLineEditBox::moveCursorUp()
{
    size_t currentIdx = textCursor_.getIndex();
    size_t currentLine = getLineFromIndex(currentIdx);
    
    if (currentLine > 0)
    {
        size_t column = getColumnFromIndex(currentIdx);
        size_t newIdx = getIndexFromLineColumn(currentLine - 1, column);
        textCursor_.setIndex(newIdx);
        updateCursorAndSelection(false);
    }
}

void MultiLineEditBox::moveCursorDown()
{
    size_t currentIdx = textCursor_.getIndex();
    size_t currentLine = getLineFromIndex(currentIdx);
    
    if (currentLine < lineIndices_.size() - 1)
    {
        size_t column = getColumnFromIndex(currentIdx);
        size_t newIdx = getIndexFromLineColumn(currentLine + 1, column);
        textCursor_.setIndex(newIdx);
        updateCursorAndSelection(false);
    }
}

void MultiLineEditBox::moveCursorHome()
{
    size_t currentIdx = textCursor_.getIndex();
    size_t currentLine = getLineFromIndex(currentIdx);
    
    size_t newIdx = lineIndices_[currentLine].startIndex;
    textCursor_.setIndex(newIdx);
    updateCursorAndSelection(false);
}

void MultiLineEditBox::moveCursorEnd()
{
    size_t currentIdx = textCursor_.getIndex();
    size_t currentLine = getLineFromIndex(currentIdx);
    
    size_t newIdx = lineIndices_[currentLine].startIndex + lineIndices_[currentLine].length;
    textCursor_.setIndex(newIdx);
    updateCursorAndSelection(false);
}

void MultiLineEditBox::insertNewLine()
{
    std::string newText = text_.getText();
    
    if (not selection_.isEmpty())
    {
        newText.erase(selection_.startsAt(), selection_.length());
        textCursor_.setIndex(selection_.startsAt());
        selection_.clear();
    }
    
    if (newText.length() < maxLength_)
    {
        newText.insert(textCursor_.getIndex(), 1, '\n');
        text_.setText(newText);
        rebuildLineIndices();
        updateTextVisbleArea();
        textCursor_.moveRight(false);
    }
}

void MultiLineEditBox::deleteNewLine()
{
    std::string newText = text_.getText();
    
    if (textCursor_.getIndex() < newText.length() && newText[textCursor_.getIndex()] == '\n')
    {
        newText.erase(textCursor_.getIndex(), 1);
        text_.setText(newText);
        rebuildLineIndices();
        updateTextVisbleArea();
    }
}

EventStatus MultiLineEditBox::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    if (not isInside(mouseButtonPressedEvent.position))
    {
        return EventStatus::NotConsumed;
    }

    if (mouseButtonPressedEvent.button != gui::event::MouseButton::Left)
    {
        return EventStatus::NotConsumed;
    }

    mouseLeftButtonPressed_ = true;

    focus();
    enterEdit();

    // Convert 2D screen position to text index
    sf::Vector2f screenPos{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};
    size_t textIndex = getIndexFromScreenPosition(screenPos);
    textCursor_.setIndex(textIndex);

    if (anyShiftHeldDown_)
    {
        selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
    }
    else
    {
        if (not selection_.isEmpty())
        {
            selection_.clear();
            text_.updateTexture();
        }
        selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
    }

    return EventStatus::NotConsumed;
}

EventStatus MultiLineEditBox::on(const event::MouseMoved& mouseMovedEvent)
{
    if(mouseLeftButtonPressed_)
    {
        // Convert 2D screen position to text index
        sf::Vector2f screenPos{mouseMovedEvent.position.x, mouseMovedEvent.position.y};
        size_t textIndex = getIndexFromScreenPosition(screenPos);
        textCursor_.setIndex(textIndex);
        selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
    }

    return EventStatus::NotConsumed;
}

EventStatus MultiLineEditBox::on(const event::KeyboardKeyPressed& keyboardKeyPressed)
{
    auto result = EventStatus::NotConsumed;
    if(not isFocused()) return result;

    std::string newText = text_.getText();

    switch (keyboardKeyPressed.key)
    {
        case sf::Keyboard::Delete :
        {
            if (not selection_.isEmpty())
            {
                newText.erase(selection_.startsAt(), selection_.length());
                textCursor_.setIndex(selection_.startsAt());
                selection_.clear();
                anyShiftHeldDown_ = false;
                result = EventStatus::Consumed;
            }
            else if (not newText.empty() && textCursor_.getIndex() < newText.length())
            {
                newText.erase(textCursor_.getIndex(), 1);
                result = EventStatus::Consumed;
            }

            text_.setText(newText);
            rebuildLineIndices();
            break;
        }

        case sf::Keyboard::Return :
        {
            insertNewLine();
            return EventStatus::Consumed;
        }

        case sf::Keyboard::Up :
        {
            moveCursorUp();
            constexpr bool AT_START_OF_SELECTION_ON_CANCEL = false;
            updateCursorAndSelection(AT_START_OF_SELECTION_ON_CANCEL);
            result = EventStatus::Consumed;
            break;
        }

        case sf::Keyboard::Down :
        {
            moveCursorDown();
            constexpr bool AT_END_OF_SELECTION_ON_CANCEL = true;
            updateCursorAndSelection(AT_END_OF_SELECTION_ON_CANCEL);
            result = EventStatus::Consumed;
            break;
        }

        case sf::Keyboard::Home :
        {
            moveCursorHome();
            constexpr bool AT_START_OF_SELECTION_ON_CANCEL = false;
            updateCursorAndSelection(AT_START_OF_SELECTION_ON_CANCEL);
            result = EventStatus::Consumed;
            break;
        }

        case sf::Keyboard::End :
        {
            moveCursorEnd();
            constexpr bool AT_END_OF_SELECTION_ON_CANCEL = true;
            updateCursorAndSelection(AT_END_OF_SELECTION_ON_CANCEL);
            result = EventStatus::Consumed;
            break;
        }

        default:
        {
            // Delegate to base class for common keys
            result = BaseEditBox::on(keyboardKeyPressed);
            
            // Handle Backspace special case - need to rebuild line indices
            if (keyboardKeyPressed.key == sf::Keyboard::Backspace && result == EventStatus::Consumed)
            {
                rebuildLineIndices();
            }
            return result;
        }
    }

    updateTextVisbleArea();
    return result;
}

EventStatus MultiLineEditBox::on(const event::TextEntered& textEntered)
{
    if(not isFocused()) return EventStatus::NotConsumed;

    std::string text = text_.getText();

    if (text.length() >= maxLength_) return EventStatus::NotConsumed;

    // FIXME: crude unicode conversion and check to for printable characters
    if (textEntered.unicode >= 0x20 && textEntered.unicode < 0x7F)
    {

        if (not selection_.isEmpty())
        {
            text.replace(selection_.startsAt(), selection_.length(), 1, static_cast<char>(textEntered.unicode));
            textCursor_.setIndex(selection_.startsAt());
            selection_.clear();
        }
        else
        {
            text.insert(textCursor_.getIndex(), 1, static_cast<char>(textEntered.unicode));
        }

        text_.setText(text);
        rebuildLineIndices();
        updateTextVisbleArea();
        textCursor_.moveRight(false);
        return EventStatus::Consumed;
    }

    return EventStatus::NotConsumed;
}

}  // namespace gui
