#include "gui/BaseEditBox.hpp"

#include <fmt/format.h>

#include "gui/Clipboard.hpp"
#include "gui/Debug.hpp"
#include "gui/StyleSheet.hpp"
#include "gui/TextureLibrary.hpp"

namespace 
{
const sf::Vector2f EXTRA_END_OFFSET{6.f, 4.f};
}  // namespace

namespace gui
{

BaseEditBox::~BaseEditBox() = default;

BaseEditBox::BaseEditBox()
: background_{gui::FramedSprite::LayoutConfig{
    .cornerSizes = {.topLeft = {4.f, 4.f}, .bottomRight = {4.f, 4.f}},
    .uvs = gui::FramedSprite::LayoutConfig::UVs{
        .topLeft        = {0.0f,   0.0f,  2.0f, 2.0f},
        .topRight       = {4.0f,   0.0f,  2.0f, 2.0f},
        .bottomLeft     = {0.0f,   4.0f,  2.0f, 2.0f},
        .bottomRight    = {4.0f,   4.0f,  2.0f, 2.0f},
    }}}
, focusTexture_{TextureLibrary::instance().get("editbox_active")}
, normalTexture_{TextureLibrary::instance().get("editbox_inactive")}
, textCursor_{text_}
, selection_{text_}
, maxLength_{128}
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
    text_.setGlobalPosition(Component::getGlobalPosition());

    textCursor_.setFont(text_.getFont());
    textCursor_.setCharacterSize(text_.getCharacterSize());
    textCursor_.disable();

    background_.setTexture(normalTexture_);
}

std::string BaseEditBox::getText()
{
    return text_.getText();
}

void BaseEditBox::setText(const std::string_view text)
{
    text_.setText(text);
    updateTextVisbleArea();
    onTextChanged();
}

void BaseEditBox::setAlignment(const gui::Alignment& alignment)
{
    alignment_ = alignment;
    updateTextVisbleArea();
}

void BaseEditBox::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
    renderTexture.draw(text_);
}

void BaseEditBox::onSizeChange()
{
    text_.setSize(Component::getSize() - EXTRA_END_OFFSET * 2.f);
    text_.setGlobalPosition(Component::getGlobalPosition() + EXTRA_END_OFFSET);
    background_.setSize(Component::getSize());
    updateTextVisbleArea();
    textCursor_.update();
    selection_.update();
}

void BaseEditBox::onPositionChange()
{
    text_.setGlobalPosition(Component::getGlobalPosition() + EXTRA_END_OFFSET);
    background_.setPosition(getGlobalPosition());
    updateTextVisbleArea();
    textCursor_.update();
    selection_.update();
}

EventStatus BaseEditBox::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
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

EventStatus BaseEditBox::on(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
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

EventStatus BaseEditBox::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (mouseButtonReleasedEvent.button == gui::event::MouseButton::Left)
    {
        mouseLeftButtonPressed_ = false;
    }

    return EventStatus::NotConsumed;
}

EventStatus BaseEditBox::on(const event::MouseMoved&)
{
    if(mouseLeftButtonPressed_)
    {
        selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
    }

    return EventStatus::NotConsumed;
}

void BaseEditBox::cut()
{
    if (not selection_.isEmpty())
    {
        copy();
        auto textToUpdate = text_.getText();
        textToUpdate.replace(selection_.startsAt(), selection_.length(), "", 0);
        textCursor_.setIndex(selection_.startsAt());
        selection_.clear();
        text_.setText(textToUpdate);
        onTextChanged();
        updateTextVisbleArea();
    }
}

void BaseEditBox::copy()
{
    if (not selection_.isEmpty())
    {
        auto textToSave = text_.getText().substr(selection_.startsAt(), selection_.length());
        Clipboard::save(textToSave);
    }
}

void BaseEditBox::paste()
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
    onTextChanged();
    updateTextVisbleArea();
}

void BaseEditBox::startSelection()
{
    if (selection_.isEmpty())
    {
        selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
    }
}

void BaseEditBox::updateCursorAndSelection(const bool atSelectionEndOnCancel)
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

EventStatus BaseEditBox::on(const event::KeyboardKeyPressed& keyboardKeyPressed)
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
            onTextChanged();
            break;
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

EventStatus BaseEditBox::on(const event::KeyboardKeyReleased& keyboardKeyReleased)
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

EventStatus BaseEditBox::on(const event::TextEntered& textEntered)
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
        onTextChanged();
        updateTextVisbleArea();
        textCursor_.moveRight(false);
        return EventStatus::Consumed;
    }

    return EventStatus::NotConsumed;
}

void BaseEditBox::enterEdit()
{
    background_.setTexture(focusTexture_);
    textCursor_.enable();
}

EventStatus BaseEditBox::on(const gui::event::FocusLost&)
{
    background_.setTexture(normalTexture_);
    textCursor_.disable();
    selection_.clear();
    text_.updateTexture();
    return EventStatus::Consumed;
}

EventStatus BaseEditBox::on(const gui::event::FocusGained&)
{
    enterEdit();
    return EventStatus::Consumed;
}

}  // namespace gui
