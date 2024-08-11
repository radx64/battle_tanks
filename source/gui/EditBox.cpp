#include "gui/EditBox.hpp"

#include "gui/Debug.hpp"
#include "gui/keyboard/Utils.hpp"
#include "gui/StyleSheet.hpp"

constexpr float EXTRA_END_OFFSET = 5.f;
constexpr uint32_t DEFAULT_TEXT_MAX_LENGTH = 128;

namespace gui
{

EditBox::EditBox()
: max_length_{DEFAULT_TEXT_MAX_LENGTH}
{
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

std::string EditBox::getText()
{
    return text_.getText();
}

void EditBox::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
    if (isFocused()) text_cursor_.render(renderTexture);
    renderTexture.draw(text_);
}

void EditBox::onSizeChange()
{
    background_.setSize(Component::getSize());
    text_.setSize(Component::getSize());
    updateTextVisbleArea();
    text_cursor_.update(text_);
}

void EditBox::onPositionChange()
{
    text_.setGlobalPosition(Component::getGlobalPosition());
    background_.setPosition(getGlobalPosition());
    text_cursor_.update(text_);
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
        text_cursor_.moveTo(text_, mouseButtonPressedEvent.position.x);
        text_cursor_.update(text_);
        focus();
    }

    return gui::EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    UNUSED(mouseButtonReleasedEvent);
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

            text_cursor_.moveLeft();
            new_text.erase(text_cursor_.getIndex(), 1);
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
            break;
        }

        case sf::Keyboard::Right : 
        {
            text_cursor_.moveRight();
            break;
        }

        default: break;
    }

    text_.setText(new_text);
    updateTextVisbleArea();
    text_cursor_.update(text_);
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
        text.insert(text_cursor_.getIndex(), 1, static_cast<char>(textEntered.unicode));
        text_.setText(text);
        updateTextVisbleArea();
        text_cursor_.moveRight();
        text_cursor_.update(text_);
        return gui::EventStatus::Consumed;  
    }

    return gui::EventStatus::NotConsumed;  
}

EventStatus EditBox::on(const event::KeyboardKeyReleased& keyboardKeyReleased)
{
    UNUSED(keyboardKeyReleased);
    return gui::EventStatus::NotConsumed;
}

void EditBox::onFocus()
{
    background_.setFillColor(sf::Color::White);
}

void EditBox::onFocusLost()
{
    background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
}

}  // namespace gui