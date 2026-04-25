#include "gui/MultiLineEditBox.hpp"

#include <fmt/format.h>

#include "gui/Clipboard.hpp"
#include "gui/Debug.hpp"
#include "gui/FontHeightCache.hpp"
#include "gui/TextureLibrary.hpp"

namespace 
{
    const sf::Vector2f EXTRA_END_OFFSET{6.f, 4.f};
    constexpr uint32_t DEFAULT_TEXT_MAX_LENGTH = 2048;
    constexpr uint32_t DEFAULT_MAX_LINES = 10;
}  // namespace

// FIXME:
// This class is currently a bit of a mess, and needs some refactoring and cleanup.
// Cursor (caret) behaviour when going up and down need to be reworked
// View span should be update when cursor escapes it
// Not every time cursor changes position.
// Eg. in VSCode "escaping means reaching even 3 lines before end of view"
// Ensure cursor visible should go away.

namespace gui
{

std::unique_ptr<MultiLineEditBox> MultiLineEditBox::create()
{
    return std::unique_ptr<MultiLineEditBox>{new MultiLineEditBox{}};
}
MultiLineEditBox::MultiLineEditBox()
: BaseEditBox()
, maxLines_{DEFAULT_MAX_LINES}
, firstVisibleLine_{0}
{
    maxLength_ = DEFAULT_TEXT_MAX_LENGTH;
    rebuildLineIndices();
}

void MultiLineEditBox::setFirstVisibleLine(size_t line)
{
    const auto clampedLine = std::min(line, getMaxFirstVisibleLine());
    if (firstVisibleLine_ == clampedLine)
    {
        return;
    }

    firstVisibleLine_ = clampedLine;
    updateTextVisbleArea();
    notifyViewChange();
}

size_t MultiLineEditBox::getFirstVisibleLine() const
{
    return firstVisibleLine_;
}

size_t MultiLineEditBox::getVisibleLineSpan() const
{
    const sf::Font* font = text_.getFont();
    if (not font)
    {
        return 1;
    }

    const auto lineHeight = getFontLineSpacing(*font, text_.getCharacterSize());
    if (lineHeight <= 0.f)
    {
        return 1;
    }

    return std::max<size_t>(1, static_cast<size_t>(std::ceil(((text_.getSize().y / lineHeight)) - 0.5f)));
} 

size_t MultiLineEditBox::getMaxFirstVisibleLine() const
{
    const auto visibleLineSpan = getVisibleLineSpan();
    if (lineIndices_.size() <= visibleLineSpan)
    {
        return 0;
    }

    return lineIndices_.size() - visibleLineSpan;
}

size_t MultiLineEditBox::getLineCount() const
{
    return lineIndices_.size();
}

size_t MultiLineEditBox::getCursorLine() const
{
    return getLineFromIndex(textCursor_.getIndex());
}

void MultiLineEditBox::onViewChange(std::function<void()> callback)
{
    onViewChange_ = std::move(callback);
}

void MultiLineEditBox::onSizeChange()
{
    BaseEditBox::onSizeChange();
    notifyViewChange();
}

void MultiLineEditBox::updateTextVisbleArea()
{
    const sf::Font* font = text_.getFont();
    if (not font)
    {
        text_.setOffset(sf::Vector2f{0.f, 0.f});
        return;
    }

    const auto lineHeight = getFontLineSpacing(*font, text_.getCharacterSize());
    text_.setOffset(sf::Vector2f{0.f, -static_cast<float>(firstVisibleLine_) * lineHeight});
    logger_.debug(fmt::format("Updating text visible area. First visible line: {}, line height: {}, offset: {}", firstVisibleLine_, lineHeight, text_.getOffset().y));
}

void MultiLineEditBox::onTextChanged()
{
    rebuildLineIndices();
    firstVisibleLine_ = std::min(firstVisibleLine_, getMaxFirstVisibleLine());
    ensureCursorVisible();
    updateTextVisbleArea();
    notifyViewChange();
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

void MultiLineEditBox::ensureCursorVisible()
{
    const auto currentLine = getLineFromIndex(textCursor_.getIndex());
    const auto visibleLineSpan = getVisibleLineSpan();

    if (currentLine < firstVisibleLine_)
    {
        firstVisibleLine_ = currentLine;
    }
    else if (currentLine >= firstVisibleLine_ + visibleLineSpan)
    {
        firstVisibleLine_ = currentLine - visibleLineSpan + 1;
    }

    firstVisibleLine_ = std::min(firstVisibleLine_, getMaxFirstVisibleLine());
}

void MultiLineEditBox::notifyViewChange()
{
    if (onViewChange_)
    {
        onViewChange_();
    }
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
    
    // Calculate which line was clicked, accounting for scroll offset
    float relativeY = screenPos.y - textGlobalPos.y - textOffset.y;
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
        ensureCursorVisible();
        updateTextVisbleArea();
        textCursor_.update();
        updateCursorAndSelection(false);
        notifyViewChange();
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
        ensureCursorVisible();
        updateTextVisbleArea();
        textCursor_.update();
        updateCursorAndSelection(false);
        notifyViewChange();
    }
}

void MultiLineEditBox::moveCursorHome()
{
    size_t currentIdx = textCursor_.getIndex();
    size_t currentLine = getLineFromIndex(currentIdx);
    
    size_t newIdx = lineIndices_[currentLine].startIndex;
    textCursor_.setIndex(newIdx);
    ensureCursorVisible();
    updateTextVisbleArea();
    textCursor_.update();
    updateCursorAndSelection(false);
}

void MultiLineEditBox::moveCursorEnd()
{
    size_t currentIdx = textCursor_.getIndex();
    size_t currentLine = getLineFromIndex(currentIdx);
    
    size_t newIdx = lineIndices_[currentLine].startIndex + lineIndices_[currentLine].length;
    textCursor_.setIndex(newIdx);
    ensureCursorVisible();
    updateTextVisbleArea();
    textCursor_.update();
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
        textCursor_.moveRight(false);
        ensureCursorVisible();
        updateTextVisbleArea();
        textCursor_.update();
        notifyViewChange();
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
        ensureCursorVisible();
        updateTextVisbleArea();
        textCursor_.update();
        notifyViewChange();
    }
}

EventStatus MultiLineEditBox::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
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
    ensureCursorVisible();
    updateTextVisbleArea();
    textCursor_.update();
    notifyViewChange();

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
        ensureCursorVisible();
        updateTextVisbleArea();
        textCursor_.update();
        notifyViewChange();
        selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
    }

    return EventStatus::NotConsumed;
}

EventStatus MultiLineEditBox::on(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
{
    if (mouseButtonDoublePressedEvent.button != gui::event::MouseButton::Left)
    {
        return EventStatus::NotConsumed;
    }

    if (not isFocused())
    {
        focus();
        enterEdit();
    }

    const sf::Vector2f screenPos{
        mouseButtonDoublePressedEvent.position.x,
        mouseButtonDoublePressedEvent.position.y
    };
    const size_t textIndex = getIndexFromScreenPosition(screenPos);

    textCursor_.setIndex(textIndex);
    ensureCursorVisible();
    updateTextVisbleArea();
    textCursor_.update();
    notifyViewChange();
    textCursor_.moveLeft(true);
    selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
    textCursor_.moveRight(true);
    selection_.to(textCursor_.getIndex(), textCursor_.getPosition());

    return EventStatus::Consumed;
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
            if (result == EventStatus::Consumed)
            {
                ensureCursorVisible();
                updateTextVisbleArea();
                textCursor_.update();
                notifyViewChange();
            }
            return result;
        }
    }

    ensureCursorVisible();
    updateTextVisbleArea();
    textCursor_.update();
    notifyViewChange();
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
        textCursor_.moveRight(false);
        ensureCursorVisible();
        updateTextVisbleArea();
        textCursor_.update();
        notifyViewChange();
        return EventStatus::Consumed;
    }

    return EventStatus::NotConsumed;
}

}  // namespace gui
