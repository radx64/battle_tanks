#include "gui/EditBox.hpp"

#include "gui/Debug.hpp"
#include "gui/keyboard/Utils.hpp"
#include "gui/StyleSheet.hpp"

constexpr float EXTRA_END_OFFSET = 5.f;
constexpr uint32_t DEFAULT_TEXT_MAX_LENGTH = 128;

namespace gui
{

EditBox::EditBox()
: text_{}
, text_cursor_{text_}
, selection_{text_}
, max_length_{DEFAULT_TEXT_MAX_LENGTH}
{
    text_.addModifier(&selection_);
    text_.addModifier(&text_cursor_);

    auto style = BasicStyleSheetFactory::instance();
    text_.setFont(style.getFont());
    text_.setCharacterSize(style.getFontSize());
    text_.setFillColor(style.getFontColor());
    text_.setOutlineColor(style.getOutlineColor());
    text_.setOutlineThickness(style.getOutlineThickness());
    text_.setGlobalPosition(Component::getGlobalPosition());

    text_cursor_.setFont(text_.getFont());
    text_cursor_.setCharacterSize(text_.getCharacterSize());

    background_.setFillColor(style.getWindowColor());
    background_.setOutlineColor(style.getOutlineColor());
    background_.setOutlineThickness(style.getOutlineThickness()); 
    background_.setPosition(getGlobalPosition());
    background_.setSize(Component::getSize());
}

EditBox::~EditBox()
{
    text_.removeModifier(&selection_);
    text_.removeModifier(&text_cursor_);
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
    text_cursor_.update();
    selection_.update();
}

void EditBox::onPositionChange()
{
    text_.setGlobalPosition(Component::getGlobalPosition());
    background_.setPosition(getGlobalPosition());
    updateTextVisbleArea();
    text_cursor_.update();
    selection_.update();
}

void EditBox::updateTextVisbleArea()
{
    float text_x_offset = text_.getSize().x - text_.getTextWidth();
    text_x_offset -= EXTRA_END_OFFSET;

    if (text_x_offset < 0)
    {
        text_.setOffset({text_x_offset, 0.f});
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
        text_cursor_.moveTo(mouseButtonPressedEvent.position.x);
        text_cursor_.update();
        focus();

        if (not selection_.isOngoing())
        {
            selection_.start(text_cursor_.getIndex(), text_cursor_.getPosition());
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
    if(selection_.isOngoing())
    {
        text_cursor_.moveTo(mouseMovedEvent.position.x);
        text_cursor_.update();
        selection_.updateEnd(text_cursor_.getIndex(), text_cursor_.getPosition());
        selection_.update();
    }
        
    return gui::EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::KeyboardKeyPressed& keyboardKeyPressed)
{
    if(not isFocused()) return gui::EventStatus::NotConsumed;

    std::string new_text = text_.getText();

    switch (keyboardKeyPressed.key)
    {
        case sf::Keyboard::Backspace :
        {
            if (new_text.empty() || text_cursor_.getIndex() == 0) 
            {
                return gui::EventStatus::NotConsumed;
            }
            
            if (!selection_.isEmpty())
            {
                new_text.erase(selection_.startsAt(), selection_.length());
                text_cursor_.setIndex(selection_.startsAt());
                text_cursor_.update();
                selection_.clear();

            }
            else
            {
                text_cursor_.moveLeft();
                text_cursor_.update();
                new_text.erase(text_cursor_.getIndex(), 1);
            }

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
            text_cursor_.moveLeft();
            text_cursor_.update();

            if (selection_.isOngoing())
            {
                selection_.updateEnd(text_cursor_.getIndex(), text_cursor_.getPosition());
                selection_.update();
            }
            break;
        }

        case sf::Keyboard::Right : 
        {
            text_cursor_.moveRight();
            text_cursor_.update();
    
            if (selection_.isOngoing())
            {
                selection_.updateEnd(text_cursor_.getIndex(), text_cursor_.getPosition());
                selection_.update();
            }
            break;
        }

        case sf::Keyboard::LShift :
        {
            if (not selection_.isOngoing())
            {
                //TODO: extract this as it is same as in mouse handling
                selection_.start(text_cursor_.getIndex(), text_cursor_.getPosition());
                selection_.update();
            }
            break;
        }

        default: break;
    }

    text_.setText(new_text);
    updateTextVisbleArea();
    return gui::EventStatus::Consumed;
}

EventStatus EditBox::on(const event::TextEntered& textEntered)
{
    if(not isFocused()) return gui::EventStatus::NotConsumed;

    std::string text = text_.getText();

    if (text.length() >= max_length_) return gui::EventStatus::NotConsumed;

    // FIXME: crude unicode conversion, backspace and tab ignore (0x8, 0x9) 
    if (textEntered.unicode < 128 && textEntered.unicode != 0x8 && textEntered.unicode != 0x9)
    {
        if (selection_.isOngoing())
        {
            selection_.end();
        }

        if (not selection_.isEmpty())
        {
            text.replace(selection_.startsAt(), selection_.length(), 1, static_cast<char>(textEntered.unicode));
            selection_.clear();
            selection_.update();
            text_cursor_.setIndex(selection_.startsAt());
            text_cursor_.update();
        }
        else
        {
            text.insert(text_cursor_.getIndex(), 1, static_cast<char>(textEntered.unicode));
        }

        text_.setText(text);
        updateTextVisbleArea();
        text_cursor_.moveRight();
        text_cursor_.update();
        return gui::EventStatus::Consumed;  
    }

    return gui::EventStatus::NotConsumed;  
}

EventStatus EditBox::on(const event::KeyboardKeyReleased& keyboardKeyReleased)
{
    if(not isFocused()) return gui::EventStatus::NotConsumed;
    
    switch (keyboardKeyReleased.key)
    {
        case sf::Keyboard::LShift :
        {
            if (selection_.isOngoing())
            {
                selection_.end();
            }
            return gui::EventStatus::Consumed; 
        }
        default : return gui::EventStatus::NotConsumed;  
    }

    return gui::EventStatus::NotConsumed;
}

void EditBox::onFocus()
{
    background_.setFillColor(sf::Color::White);
    text_cursor_.enable();
}

void EditBox::onFocusLost()
{
    background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
    text_cursor_.disable();
    selection_.clear();
    text_.updateTexture();
}

}  // namespace gui
