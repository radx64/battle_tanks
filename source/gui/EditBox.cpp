#include "gui/EditBox.hpp"

#include <fmt/format.h>

#include "gui/Clipboard.hpp"
#include "gui/Debug.hpp"
#include "gui/keyboard/Utils.hpp"
#include "gui/StyleSheet.hpp"


const sf::Vector2f EXTRA_END_OFFSET{2.f, 2.f};
constexpr uint32_t DEFAULT_TEXT_MAX_LENGTH = 128;
constexpr bool ALIGN_TEXT_TO_BASELINE = true;

/*
    TODO: 
    Cursor position is not taken into account when calculating alignment offset
    Add repetable key press for both moving cursor and entering text

*/

namespace gui
{
EditBox::EditBox()
: text_{ALIGN_TEXT_TO_BASELINE}
, textCursor_{text_}
, selection_{text_}
, maxLength_{DEFAULT_TEXT_MAX_LENGTH}
, anyShiftHeldDown_{false}
, mouseLeftButtonPressed_{false}
, alignment_{gui::Alignment::Left}
{
    enableFocus();

    text_.addModifier(&selection_);
    text_.addModifier(&textCursor_);

    auto style = BasicStyleSheetFactory::instance();
    text_.setFont(style.getFont());
    text_.setCharacterSize(style.getFontSize());
    text_.setFillColor(style.getFontColor());
    text_.setOutlineColor(style.getOutlineColor());
    text_.setOutlineThickness(style.getOutlineThickness());
    text_.setGlobalPosition(Component::getGlobalPosition());

    textCursor_.setFont(text_.getFont());
    textCursor_.setCharacterSize(text_.getCharacterSize());
    textCursor_.disable();

    backgroundShape_.setFillColor(style.getWindowColor());
    backgroundShape_.setOutlineColor(style.getOutlineColor());
    backgroundShape_.setOutlineThickness(style.getOutlineThickness());
    backgroundShape_.setPosition(getGlobalPosition());
    backgroundShape_.setSize(Component::getSize());
}

std::string EditBox::getText()
{
    return text_.getText();
}

void EditBox::setText(const std::string_view text)
{
    text_.setText(text);
    updateTextVisbleArea();
}

void EditBox::setAlignment(const gui::Alignment& alignment)
{
    alignment_ = alignment;
    updateTextVisbleArea();
}

void EditBox::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(backgroundShape_);
    renderTexture.draw(text_);
}

void EditBox::onSizeChange()
{
    text_.setSize(Component::getSize() - EXTRA_END_OFFSET*2.f);
    text_.setGlobalPosition(Component::getGlobalPosition());
    backgroundShape_.setSize(Component::getSize());
    updateTextVisbleArea();
    textCursor_.update();
    selection_.update();
}

void EditBox::onPositionChange()
{
    text_.setGlobalPosition(Component::getGlobalPosition());
    backgroundShape_.setPosition(getGlobalPosition());
    updateTextVisbleArea();
    textCursor_.update();
    selection_.update();
}

void EditBox::updateTextVisbleArea()
{
    float textXoffset = text_.getSize().x - text_.getTextBounds().width;

    sf::Vector2f textSize = boundsToSize(text_.getTextBounds());

    auto alignmentOffset = calculateAlignmentOffset(getSize(), textSize, alignment_);

    // if text does no fit (alignment offset is negative)
    // ignore it and behave as normal left alignment so cursor
    // will be on the right side of the text and stil visible

    if (alignmentOffset.x < 0) alignmentOffset.x = 0;

    if (textXoffset < 0)
    {
        text_.setOffset(sf::Vector2f{textXoffset, 0.f} + alignmentOffset);
    }
    else
    {
        text_.setOffset(alignmentOffset);
    }
}

EventStatus EditBox::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
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

    textCursor_.moveTo(mouseButtonPressedEvent.position.x);

    if (anyShiftHeldDown_)
    {
        selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
    }
    else
    {
        if (not selection_.isEmpty())
        {
            selection_.clear();
        }
        selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
    }

    return EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
{
    if (not isInside(mouseButtonDoublePressedEvent.position))
    {
        return EventStatus::NotConsumed;
    }

    textCursor_.moveLeft(true);
    selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
    textCursor_.moveRight(true);
    selection_.to(textCursor_.getIndex(), textCursor_.getPosition());

    return EventStatus::Consumed;
}

EventStatus EditBox::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (mouseButtonReleasedEvent.button == gui::event::MouseButton::Left)
    {
        mouseLeftButtonPressed_ = false;
    }

    return EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::MouseMoved& mouseMovedEvent)
{
    if(mouseLeftButtonPressed_)
    {
        textCursor_.moveTo(mouseMovedEvent.position.x);
        selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
    }

    return EventStatus::NotConsumed;
}

void EditBox::cut()
{
    if (not selection_.isEmpty())
    {
        copy();
        auto textToUpdate = text_.getText();
        textToUpdate.replace(selection_.startsAt(), selection_.length(), "", 0);
        textCursor_.setIndex(selection_.startsAt());
        selection_.clear();
        text_.setText(textToUpdate);
        updateTextVisbleArea();
    }
}

void EditBox::copy()
{
    if (not selection_.isEmpty())
    {
        auto textToSave = text_.getText().substr(selection_.startsAt(), selection_.length());
        Clipboard::save(textToSave);
    }
}

void EditBox::paste()
{
    auto textToPaste = Clipboard::retreive();
    auto textToUpdate = text_.getText();
    if (not selection_.isEmpty())
    {
        textToUpdate.replace(selection_.startsAt(), selection_.length(),textToPaste.c_str(), textToPaste.size());
        selection_.clear();
        text_.setText(textToUpdate);
    }
    else
    {
        textToUpdate.insert(textCursor_.getIndex(), textToPaste.c_str(), textToPaste.size());
        text_.setText(textToUpdate);
    }
    textCursor_.setIndex(textCursor_.getIndex() + textToPaste.size());
    updateTextVisbleArea();
}

void EditBox::startSelection()
{
    if (selection_.isEmpty())
    {
        selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
    }
}

void EditBox::updateCursorAndSelection(const bool atSelectionEndOnCancel)
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
    }
}

EventStatus EditBox::on(const event::KeyboardKeyPressed& keyboardKeyPressed)
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
                newText.erase(textCursor_.getIndex(), 1);
                result = EventStatus::Consumed;
            }

            text_.setText(newText);
            break;
        }

        case sf::Keyboard::Return :
        {
            defocusWithAllChildren();
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

EventStatus EditBox::on(const event::KeyboardKeyReleased& keyboardKeyRelased)
{
    if (not isFocused()) return EventStatus::NotConsumed;

    switch (keyboardKeyRelased.key)
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

EventStatus EditBox::on(const event::TextEntered& textEntered)
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
        updateTextVisbleArea();
        textCursor_.moveRight(false);
        return EventStatus::Consumed;
    }

    return EventStatus::NotConsumed;
}

void EditBox::enterEdit()
{
    backgroundShape_.setFillColor(sf::Color::White);
    textCursor_.enable();
}

EventStatus EditBox::on(const gui::event::FocusLost&)
{
    backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
    textCursor_.disable();
    selection_.clear();
    text_.updateTexture();
    return EventStatus::Consumed;
}

EventStatus EditBox::on(const gui::event::FocusGained&)
{
    enterEdit();
    return EventStatus::Consumed;
}

}  // namespace gui
