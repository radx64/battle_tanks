#include "gui/EditBox.hpp"

#include "gui/Clipboard.hpp"
#include "gui/Debug.hpp"
#include "gui/keyboard/Utils.hpp"
#include "gui/StyleSheet.hpp"

constexpr float EXTRA_END_OFFSET = 5.f;
constexpr uint32_t DEFAULT_TEXT_MAX_LENGTH = 128;

namespace gui
{
EditBox::EditBox()
: text_{}
, textCursor_{text_}
, selection_{text_}
, maxLength_{DEFAULT_TEXT_MAX_LENGTH}
, anyShiftHeldDown_{false}
, mouseLeftButtonPressed_{false}
{
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

    background_.setFillColor(style.getWindowColor());
    background_.setOutlineColor(style.getOutlineColor());
    background_.setOutlineThickness(style.getOutlineThickness());
    background_.setPosition(getGlobalPosition());
    background_.setSize(Component::getSize());
}

EditBox::~EditBox()
{
    text_.removeModifier(&selection_);
    text_.removeModifier(&textCursor_);
}

std::string EditBox::getText()
{
    return text_.getText();
}

void EditBox::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
    renderTexture.draw(text_);
}

void EditBox::onSizeChange()
{
    text_.setGlobalPosition(Component::getGlobalPosition());
    background_.setSize(Component::getSize());
    text_.setSize(Component::getSize());
    updateTextVisbleArea();
    textCursor_.update();
    selection_.update();
}

void EditBox::onPositionChange()
{
    text_.setGlobalPosition(Component::getGlobalPosition());
    background_.setPosition(getGlobalPosition());
    updateTextVisbleArea();
    textCursor_.update();
    selection_.update();
}

void EditBox::updateTextVisbleArea()
{
    float textXoffset = text_.getSize().x - text_.getTextWidth();
    textXoffset -= EXTRA_END_OFFSET;

    if (textXoffset < 0)
    {
        text_.setOffset({textXoffset, 0.f});
    }
    else
    {
        text_.setOffset({0.f, 0.f});
    }
}

EventStatus EditBox::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    if (not isInside(mouseButtonPressedEvent.position))
    {
        return gui::EventStatus::NotConsumed;
    }

    if (mouseButtonPressedEvent.button != gui::event::MouseButton::Left)
    {
        return gui::EventStatus::NotConsumed;
    }

    mouseLeftButtonPressed_ = true;

    focus();
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

    return gui::EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (mouseButtonReleasedEvent.button == gui::event::MouseButton::Left)
    {
        mouseLeftButtonPressed_ = false;
    }

    UNUSED(mouseButtonReleasedEvent);
    return gui::EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::MouseMoved& mouseMovedEvent)
{
    if(mouseLeftButtonPressed_)
    {
        textCursor_.moveTo(mouseMovedEvent.position.x);
        selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
    }

    return gui::EventStatus::NotConsumed;
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
    if(not isFocused()) return gui::EventStatus::NotConsumed;

    std::string newText = text_.getText();

    switch (keyboardKeyPressed.key)
    {
        case sf::Keyboard::LShift :
        case sf::Keyboard::RShift :
        {
            anyShiftHeldDown_ = true;
            startSelection();
            return gui::EventStatus::Consumed;
        }

        case sf::Keyboard::Backspace :
        {
            if (not selection_.isEmpty())
            {
                newText.erase(selection_.startsAt(), selection_.length());
                textCursor_.setIndex(selection_.startsAt());
                selection_.clear();
                anyShiftHeldDown_ = false;
            }
            else if (newText.empty() || textCursor_.getIndex() == 0)
            {
                return gui::EventStatus::NotConsumed;
            }
            else
            {
                textCursor_.moveLeft(false);
                newText.erase(textCursor_.getIndex(), 1);
            }

            text_.setText(newText);
            break;
        }
        case sf::Keyboard::Tab :
        {
            defocus();
            return gui::EventStatus::Consumed;
        }

        case sf::Keyboard::Return :
        {
            defocus();
            return gui::EventStatus::Consumed;
        }

        case sf::Keyboard::Left :
        {
            textCursor_.moveLeft(keyboardKeyPressed.modifiers.control);
            constexpr bool AT_START_OF_SELECTION_ON_CANCEL = false;
            updateCursorAndSelection(AT_START_OF_SELECTION_ON_CANCEL);
            break;
        }

        case sf::Keyboard::Right :
        {
            textCursor_.moveRight(keyboardKeyPressed.modifiers.control);
            constexpr bool AT_END_OF_SELECTION_ON_CANCEL = true;
            updateCursorAndSelection(AT_END_OF_SELECTION_ON_CANCEL);
            break;
        }

        case sf::Keyboard::C : 
        {
            if (keyboardKeyPressed.modifiers.control)
            {
                copy();
            }
            break;
        }
        
        case sf::Keyboard::V : 
        {
            if (keyboardKeyPressed.modifiers.control)
            {
                paste();
            }
            break;
        }

        case sf::Keyboard::X :
        {
            if (keyboardKeyPressed.modifiers.control)
            {
                cut();
                break;
            }
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
            }
            break;
        }

        default: break;
    }

    updateTextVisbleArea();
    return gui::EventStatus::Consumed;
}

EventStatus EditBox::on(const event::KeyboardKeyReleased& keyboardKeyRelased)
{
    if (not isFocused()) return gui::EventStatus::NotConsumed;

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

    return gui::EventStatus::Consumed;
}

EventStatus EditBox::on(const event::TextEntered& textEntered)
{
    if(not isFocused()) return gui::EventStatus::NotConsumed;

    std::string text = text_.getText();

    if (text.length() >= maxLength_) return gui::EventStatus::NotConsumed;

    // FIXME: crude unicode conversion and check to for printable characters
    if (textEntered.unicode >= 0x20 && textEntered.unicode < 0x7F)
    {

        if (not selection_.isEmpty())
        {
            //selection_.end();
            text.replace(selection_.startsAt(), selection_.length(), 1, static_cast<char>(textEntered.unicode));
            selection_.clear();
            textCursor_.setIndex(selection_.startsAt());
        }
        else
        {
            text.insert(textCursor_.getIndex(), 1, static_cast<char>(textEntered.unicode));
        }

        text_.setText(text);
        updateTextVisbleArea();
        textCursor_.moveRight(false);
        return gui::EventStatus::Consumed;
    }

    return gui::EventStatus::NotConsumed;
}

void EditBox::onFocus()
{
    background_.setFillColor(sf::Color::White);
    textCursor_.enable();
}

void EditBox::onFocusLost()
{
    background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
    textCursor_.disable();
    selection_.clear();
    text_.updateTexture();
}

}  // namespace gui
