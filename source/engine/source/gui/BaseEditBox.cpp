#include "engine/gui/BaseEditBox.hpp"

#include <fmt/format.h>

#include "engine/gui/Clipboard.hpp"
#include "engine/gui/Debug.hpp"
#include "engine/gui/TextureLibrary.hpp"
#include "engine/gui/style/StyleFactory.hpp"

namespace 
{
const sf::Vector2f EXTRA_END_OFFSET{6.f, 4.f};
}  // namespace

namespace engine::gui
{

BaseEditBox::~BaseEditBox() = default;

BaseEditBox::BaseEditBox()
: style_{style::StyleFactory::instance().editBox} 
, background_{engine::gui::FramedSprite::LayoutConfig{
    .cornerSizes = {.topLeft = {4.f, 4.f}, .bottomRight = {4.f, 4.f}},
    .uvs = engine::gui::FramedSprite::LayoutConfig::UVs{
        .topLeft        = {0.0f, 0.0f, 2.0f, 2.0f},
        .topRight       = {4.0f, 0.0f, 2.0f, 2.0f},
        .bottomLeft     = {0.0f, 4.0f, 2.0f, 2.0f},
        .bottomRight    = {4.0f, 4.0f, 2.0f, 2.0f},
    }}}
, focusTexture_{TextureLibrary::instance().get("editbox_active")}
, normalTexture_{TextureLibrary::instance().get("editbox_inactive")}
, text_{style_.text}
, textCursor_{text_}
, selection_{text_, style_.selectionColor}
, maxLength_{128}
, anyShiftHeldDown_{false}
, mouseLeftButtonPressed_{false}
, alignment_{gui::Alignment::Left}
{
    enableFocus();

    text_.addModifier(&selection_);
    text_.addModifier(&textCursor_);

    const auto position = getGlobalPosition();
    text_.setGlobalPosition(sf::Vector2f{position.x, position.y});

    textCursor_.setFont(text_.getFont());
    textCursor_.setCharacterSize(text_.getCharacterSize());
    textCursor_.disable();

    background_.setTexture(normalTexture_);
    background_.setColor(style_.backgroundColor);
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

void BaseEditBox::setAlignment(const engine::gui::Alignment& alignment)
{
    alignment_ = alignment;
    updateTextVisbleArea();
}

sf::Vector2f BaseEditBox::getContentSize()
{
    const auto contentSize = text_.getTextBounds().getSize();
    return {contentSize.x + EXTRA_END_OFFSET.x, contentSize.y};
}

void BaseEditBox::applyOffset(const sf::Vector2f& offset)
{
    text_.setOffset(offset);
}

void BaseEditBox::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
    renderTexture.draw(text_);
}

void BaseEditBox::onSizeChange()
{
    const auto size = Widget::getSize();
    const auto position = Widget::getGlobalPosition();

    text_.setSize(sf::Vector2f{size.x, size.y} - EXTRA_END_OFFSET * 2.f);
    text_.setGlobalPosition(sf::Vector2f{position.x, position.y} + EXTRA_END_OFFSET);
    background_.setSize(sf::Vector2f{size.x, size.y});
    updateTextVisbleArea();
    textCursor_.update();
    selection_.update();
}

void BaseEditBox::onPositionChange()
{
    const auto position = getGlobalPosition();

    text_.setGlobalPosition(sf::Vector2f{position.x, position.y} + EXTRA_END_OFFSET);
    background_.setPosition(sf::Vector2f{position.x, position.y});

    textCursor_.update();
    selection_.update();
}

EventStatus BaseEditBox::on(const engine::events::MouseButtonPressed& mouseButtonPressedEvent)
{
    if (mouseButtonPressedEvent.getButton() != engine::core::MouseButton::Left)
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

EventStatus BaseEditBox::on(const engine::gui::events::MouseButtonDoublePressed&)
{
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

EventStatus BaseEditBox::on(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (mouseButtonReleasedEvent.getButton() == engine::core::MouseButton::Left)
    {
        mouseLeftButtonPressed_ = false;
    }

    return EventStatus::NotConsumed;
}

EventStatus BaseEditBox::on(const engine::events::MouseMoved&)
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

EventStatus BaseEditBox::on(const engine::events::KeyPressed& keyboardKeyPressed)
{
    auto result = EventStatus::NotConsumed;
    if(not isFocused()) return result;

    std::string newText = text_.getText();

    switch (keyboardKeyPressed.getKeyCode())
    {
        case engine::core::KeyCode::LShift :
        case engine::core::KeyCode::RShift :
        {
            anyShiftHeldDown_ = true;
            startSelection();
            return EventStatus::Consumed;
        }

        case engine::core::KeyCode::Backspace :
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

        case engine::core::KeyCode::Left :
        {
            textCursor_.moveLeft(keyboardKeyPressed.getModifiers().control);
            constexpr bool AT_START_OF_SELECTION_ON_CANCEL = false;
            updateCursorAndSelection(AT_START_OF_SELECTION_ON_CANCEL);
            result = EventStatus::Consumed;
            break;
        }

        case engine::core::KeyCode::Right :
        {
            textCursor_.moveRight(keyboardKeyPressed.getModifiers().control);
            constexpr bool AT_END_OF_SELECTION_ON_CANCEL = true;
            updateCursorAndSelection(AT_END_OF_SELECTION_ON_CANCEL);
            result = EventStatus::Consumed;
            break;
        }

        case engine::core::KeyCode::C :
        {
            if (keyboardKeyPressed.getModifiers().control)
            {
                copy();
                result = EventStatus::Consumed;
            }
            break;
        }

        case engine::core::KeyCode::V :
        {
            if (keyboardKeyPressed.getModifiers().control)
            {
                paste();
                result = EventStatus::Consumed;
            }
            break;
        }

        case engine::core::KeyCode::X :
        {
            if (keyboardKeyPressed.getModifiers().control)
            {
                cut();
                result = EventStatus::Consumed;
            }
            break;
        }

        case engine::core::KeyCode::A :
        {
            if (keyboardKeyPressed.getModifiers().control)
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

EventStatus BaseEditBox::on(const engine::events::KeyReleased& keyboardKeyReleased)
{
    if (not isFocused()) return EventStatus::NotConsumed;

    switch (keyboardKeyReleased.getKeyCode())
    {
        case engine::core::KeyCode::LShift :
        case engine::core::KeyCode::RShift :
        {
            anyShiftHeldDown_ = false;
            break;
        }
        default :
            break;
    }

    return EventStatus::NotConsumed;
}

EventStatus BaseEditBox::on(const engine::events::TextEntered& textEntered)
{
    if(not isFocused()) return EventStatus::NotConsumed;

    std::string text = text_.getText();

    if (text.length() >= maxLength_) return EventStatus::NotConsumed;

    // FIXME: crude unicode conversion and check to for printable characters
    if (textEntered.getUnicode() >= 0x20 && textEntered.getUnicode() < 0x7F)
    {

        if (not selection_.isEmpty())
        {
            text.replace(selection_.startsAt(), selection_.length(), 1, static_cast<char>(textEntered.getUnicode()));
            textCursor_.setIndex(selection_.startsAt());
            selection_.clear();
        }
        else
        {
            text.insert(textCursor_.getIndex(), 1, static_cast<char>(textEntered.getUnicode()));
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
    background_.setColor(style_.focusedBackgroundColor);
    textCursor_.enable();
}

EventStatus BaseEditBox::on(const engine::gui::events::FocusLost&)
{
    background_.setTexture(normalTexture_);
    background_.setColor(style_.backgroundColor);
    textCursor_.disable();
    selection_.clear();
    text_.updateTexture();
    return EventStatus::Consumed;
}

EventStatus BaseEditBox::on(const engine::gui::events::FocusGained&)
{
    enterEdit();
    return EventStatus::Consumed;
}

}  // namespace engine::gui
