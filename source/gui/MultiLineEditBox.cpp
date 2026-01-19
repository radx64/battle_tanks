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

gui::FramedSprite::LayoutConfig buildLayoutConfig(const sf::Vector2f& cornerSizes, const gui::FramedSprite::LayoutConfig::UVs& uvs)
{
    gui::FramedSprite::LayoutConfig layoutConfig{
        .cornerSizes = 
        {
            .topLeft        = {cornerSizes.x, cornerSizes.y},
            .bottomRight    = {cornerSizes.x, cornerSizes.y}
        },
        .uvs = uvs
    };

    return layoutConfig;
} 

gui::FramedSprite::LayoutConfig::UVs buildUVsForMultiLineEditBoxTexture()
{
    return gui::FramedSprite::LayoutConfig::UVs
    {
        .topLeft        = {0.0f,   0.0f,  2.0f, 2.0f},
        .topRight       = {4.0f,   0.0f,  2.0f, 2.0f},
        .bottomLeft     = {0.0f,   4.0f,  2.0f, 2.0f},
        .bottomRight    = {4.0f,   4.0f,  2.0f, 2.0f},
    };
}

gui::FramedSprite::LayoutConfig buildLayoutConfigForMultiLineEditBoxTexture()
{
    static auto layout = buildLayoutConfig({4.f, 4.f}, buildUVsForMultiLineEditBoxTexture());
    return layout;
}

}  // namespace

namespace gui
{

std::unique_ptr<MultiLineEditBox> MultiLineEditBox::create()
{
    return std::unique_ptr<MultiLineEditBox>{new MultiLineEditBox{}};
}

MultiLineEditBox::MultiLineEditBox()
: background_{buildLayoutConfigForMultiLineEditBoxTexture()}
, focusTexture_{TextureLibrary::instance().get("editbox_active")}
, normalTexture_{TextureLibrary::instance().get("editbox_inactive")}
, textCursor_{text_}
, selection_{text_}
, maxLength_{DEFAULT_TEXT_MAX_LENGTH}
, anyShiftHeldDown_{false}
, mouseLeftButtonPressed_{false}
, alignment_{gui::Alignment::Left}
, maxLines_{DEFAULT_MAX_LINES}
{
    enableFocus();

    text_.addModifier(&selection_);
    text_.addModifier(&textCursor_);

    auto style = BasicStyleSheetFactory::instance();
    text_.setFont(style.getFont());
    text_.setCharacterSize(style.getFontSize());
    text_.setFillColor(style.getFontColor());
    text_.setOutlineColor(style.getOutlineColor());
    text_.setGlobalPosition(Component::getGlobalPosition());

    textCursor_.setFont(text_.getFont());
    textCursor_.setCharacterSize(text_.getCharacterSize());
    textCursor_.disable();

    background_.setTexture(normalTexture_);
    
    rebuildLineIndices();
}

std::string MultiLineEditBox::getText()
{
    return text_.getText();
}

void MultiLineEditBox::setText(const std::string_view text)
{
    text_.setText(text);
    rebuildLineIndices();
    updateTextVisbleArea();
}

void MultiLineEditBox::setAlignment(const gui::Alignment& alignment)
{
    alignment_ = alignment;
    updateTextVisbleArea();
}

void MultiLineEditBox::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
    renderTexture.draw(text_);
}

void MultiLineEditBox::onSizeChange()
{
    text_.setSize(Component::getSize() - EXTRA_END_OFFSET * 2.f);
    text_.setGlobalPosition(Component::getGlobalPosition() + EXTRA_END_OFFSET);
    background_.setSize(Component::getSize());
    updateTextVisbleArea();
    textCursor_.update();
    selection_.update();
}

void MultiLineEditBox::onPositionChange()
{
    text_.setGlobalPosition(Component::getGlobalPosition() + EXTRA_END_OFFSET);
    background_.setPosition(getGlobalPosition());
    updateTextVisbleArea();
    textCursor_.update();
    selection_.update();
}

void MultiLineEditBox::updateTextVisbleArea()
{
    // For multiline text, we typically use top-left alignment
    // with vertical scrolling capability (simplified for now)
    text_.setOffset(sf::Vector2f{0.f, 0.f});
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

EventStatus MultiLineEditBox::on(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
{
    if (not isInside(mouseButtonDoublePressedEvent.position))
    {
        return EventStatus::NotConsumed;
    }

    if (not isFocused())
    {
        focus();
        enterEdit();
    }

    textCursor_.moveLeft(true);
    selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
    textCursor_.moveRight(true);
    selection_.to(textCursor_.getIndex(), textCursor_.getPosition());

    return EventStatus::Consumed;
}

EventStatus MultiLineEditBox::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (mouseButtonReleasedEvent.button == gui::event::MouseButton::Left)
    {
        mouseLeftButtonPressed_ = false;
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

void MultiLineEditBox::cut()
{
    if (not selection_.isEmpty())
    {
        copy();
        auto textToUpdate = text_.getText();
        textToUpdate.replace(selection_.startsAt(), selection_.length(), "", 0);
        textCursor_.setIndex(selection_.startsAt());
        selection_.clear();
        text_.setText(textToUpdate);
        rebuildLineIndices();
        updateTextVisbleArea();
    }
}

void MultiLineEditBox::copy()
{
    if (not selection_.isEmpty())
    {
        auto textToSave = text_.getText().substr(selection_.startsAt(), selection_.length());
        Clipboard::save(textToSave);
    }
}

void MultiLineEditBox::paste()
{
    auto textToPaste = Clipboard::retreive();
    auto textToUpdate = text_.getText();
    
    if (textToUpdate.length() + textToPaste.size() > maxLength_)
    {
        return;  // Would exceed max length
    }
    
    if (not selection_.isEmpty())
    {
        textToUpdate.replace(selection_.startsAt(), selection_.length(), textToPaste.c_str(), textToPaste.size());
        selection_.clear();
        text_.setText(textToUpdate);
    }
    else
    {
        textToUpdate.insert(textCursor_.getIndex(), textToPaste.c_str(), textToPaste.size());
        text_.setText(textToUpdate);
    }
    
    rebuildLineIndices();
    textCursor_.setIndex(textCursor_.getIndex() + textToPaste.size());
    updateTextVisbleArea();
}

void MultiLineEditBox::startSelection()
{
    if (selection_.isEmpty())
    {
        selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
    }
}

void MultiLineEditBox::updateCursorAndSelection(const bool atSelectionEndOnCancel)
{
    if (anyShiftHeldDown_)
    {
        selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
    }

    if (not anyShiftHeldDown_ and not selection_.isEmpty())
    {
        if (atSelectionEndOnCancel)
        {
            textCursor_.setIndex(selection_.endsAt());
        }
        else
        {
            textCursor_.setIndex(selection_.startsAt());
        }
        selection_.clear();
        text_.updateTexture();
    }
}

EventStatus MultiLineEditBox::on(const event::KeyboardKeyPressed& keyboardKeyPressed)
{
    auto result = EventStatus::NotConsumed;
    if(not isFocused()) return result;

    std::string newText = text_.getText();

    switch (keyboardKeyPressed.key)
    {
        case sf::Keyboard::LShift :
        case sf::Keyboard::RShift :
        {
            anyShiftHeldDown_ = true;
            startSelection();
            return EventStatus::Consumed;
        }

        case sf::Keyboard::Backspace :
        {
            if (not selection_.isEmpty())
            {
                newText.erase(selection_.startsAt(), selection_.length());
                textCursor_.setIndex(selection_.startsAt());
                selection_.clear();
                anyShiftHeldDown_ = false;
                result = EventStatus::Consumed;
            }
            else if (newText.empty() || textCursor_.getIndex() == 0)
            {
                return EventStatus::NotConsumed;
            }
            else
            {
                textCursor_.moveLeft(false);
                size_t idx = textCursor_.getIndex();
                newText.erase(idx, 1);
                result = EventStatus::Consumed;
            }

            text_.setText(newText);
            rebuildLineIndices();
            break;
        }

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

        case sf::Keyboard::Left :
        {
            textCursor_.moveLeft(keyboardKeyPressed.modifiers.control);
            constexpr bool AT_START_OF_SELECTION_ON_CANCEL = false;
            updateCursorAndSelection(AT_START_OF_SELECTION_ON_CANCEL);
            result = EventStatus::Consumed;
            break;
        }

        case sf::Keyboard::Right :
        {
            textCursor_.moveRight(keyboardKeyPressed.modifiers.control);
            constexpr bool AT_END_OF_SELECTION_ON_CANCEL = true;
            updateCursorAndSelection(AT_END_OF_SELECTION_ON_CANCEL);
            result = EventStatus::Consumed;
            break;
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

        case sf::Keyboard::C :
        {
            if (keyboardKeyPressed.modifiers.control)
            {
                copy();
                result = EventStatus::Consumed;
            }
            break;
        }

        case sf::Keyboard::V :
        {
            if (keyboardKeyPressed.modifiers.control)
            {
                paste();
                result = EventStatus::Consumed;
            }
            break;
        }

        case sf::Keyboard::X :
        {
            if (keyboardKeyPressed.modifiers.control)
            {
                cut();
                result = EventStatus::Consumed;
            }
            break;
        }

        case sf::Keyboard::A :
        {
            if (keyboardKeyPressed.modifiers.control)
            {
                selection_.clear();
                textCursor_.setIndex(0);
                textCursor_.update();
                selection_.start(textCursor_.getIndex(), textCursor_.getPosition());

                textCursor_.setIndex(text_.getText().length());
                textCursor_.update();
                selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
                result = EventStatus::Consumed;
            }
            break;
        }

        default: break;
    }

    updateTextVisbleArea();
    return result;
}

EventStatus MultiLineEditBox::on(const event::KeyboardKeyReleased& keyboardKeyReleased)
{
    if (not isFocused()) return EventStatus::NotConsumed;

    switch (keyboardKeyReleased.key)
    {
        case sf::Keyboard::LShift :
        case sf::Keyboard::RShift :
        {
            anyShiftHeldDown_ = false;
            break;
        }
        default :
            break;
    }

    return EventStatus::NotConsumed;
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

void MultiLineEditBox::enterEdit()
{
    background_.setTexture(focusTexture_);
    textCursor_.enable();
}

EventStatus MultiLineEditBox::on(const gui::event::FocusLost&)
{
    background_.setTexture(normalTexture_);
    textCursor_.disable();
    selection_.clear();
    text_.updateTexture();
    return EventStatus::Consumed;
}

EventStatus MultiLineEditBox::on(const gui::event::FocusGained&)
{
    enterEdit();
    return EventStatus::Consumed;
}

}  // namespace gui
