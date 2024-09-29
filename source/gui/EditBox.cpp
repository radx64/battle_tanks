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
, keyboardSelectionOngoing_{false}
, maxLength_{DEFAULT_TEXT_MAX_LENGTH}
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
    if (isInside(mouseButtonPressedEvent.position))
    {
        textCursor_.moveTo(mouseButtonPressedEvent.position.x);
        focus();

        if (not selection_.isOngoing())
        {
            selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
            selection_.update();
        }
        else
        {
            keyboardSelectionOngoing_ = false;
            selection_.clear();
            selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
            selection_.update();
        }
    }

    return gui::EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    UNUSED(mouseButtonReleasedEvent);
    selection_.end();
    return gui::EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::MouseMoved& mouseMovedEvent)
{
    if(selection_.isOngoing() and not keyboardSelectionOngoing_)
    {
        textCursor_.moveTo(mouseMovedEvent.position.x);
        selection_.updateEnd(textCursor_.getIndex(), textCursor_.getPosition());
        selection_.update();
    }

    return gui::EventStatus::NotConsumed;
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
    if (not selection_.isOngoing())
    {
        selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
        selection_.update();
    }
}

void EditBox::endSelection()
{
    if (selection_.isOngoing())
    {
        selection_.end();
    }
}

void EditBox::toggleSelection(const bool enable)
{
    if (enable)
    {
        keyboardSelectionOngoing_ = true;
        startSelection();
    }
    else
    {
        keyboardSelectionOngoing_ = false;
        endSelection();
    } 
}

void EditBox::updateCursorAndSelection(const size_t cursorIndexOnSelectionCancel)
{
    if (selection_.isOngoing())
    {
        selection_.updateEnd(textCursor_.getIndex(), textCursor_.getPosition());
        selection_.update();
    }
    else
    {
        if (not selection_.isEmpty())
        {
            textCursor_.setIndex(cursorIndexOnSelectionCancel);
            selection_.clear();
        }
    }
}

EventStatus EditBox::on(const event::KeyboardKeyPressed& keyboardKeyPressed)
{
    if(not isFocused()) return gui::EventStatus::NotConsumed;

    std::string newText = text_.getText();

    // FIXME: this toggleSelection is causing an issue when shift 
    // is released and pressed again as selection
    // is toggled of so it starts from begining
    // so this need to be redesigned
    // either start should be able to restart selection
    // or I need to figure out something else

    toggleSelection(keyboardKeyPressed.modifiers.shift);

    switch (keyboardKeyPressed.key)
    {
        case sf::Keyboard::Backspace :
        {
            if (not selection_.isEmpty())
            {
                newText.erase(selection_.startsAt(), selection_.length());
                textCursor_.setIndex(selection_.startsAt());
                selection_.clear();
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

            updateCursorAndSelection(selection_.startsAt());
            break;
        }

        case sf::Keyboard::Right :
        {
            textCursor_.moveRight(keyboardKeyPressed.modifiers.control);

            updateCursorAndSelection(selection_.endsAt());
            break;
        }

        case sf::Keyboard::C : 
        {
            if (keyboardKeyPressed.modifiers.control)
            {
                selection_.end();
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

        default: break;
    }

    updateTextVisbleArea();
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
        endSelection();

        if (not selection_.isEmpty())
        {
            text.replace(selection_.startsAt(), selection_.length(), 1, static_cast<char>(textEntered.unicode));
            selection_.clear();
            selection_.update();
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
