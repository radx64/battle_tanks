#include "gui/EditBox.hpp"

#include "gui/Debug.hpp"
#include "gui/keyboard/Utils.hpp"
#include "gui/StyleSheet.hpp"

constexpr float CURSOR_WIDTH = 3.f;

namespace gui
{

EditBox::EditBox()
{
    auto style = BasicStyleSheetFactory::instance();
    text_.setFont(style.getFont());
    text_.setCharacterSize(style.getFontSize());
    text_.setFillColor(style.getFontColor());
    text_.setOutlineColor(style.getOutlineColor());
    text_.setOutlineThickness(style.getOutlineThickness());
    setPosition({0.0f, 0.0f});
    text_.setGlobalPosition(Component::getGlobalPosition());

    background_.setFillColor(style.getWindowColor());
    background_.setOutlineColor(style.getOutlineColor());
    background_.setOutlineThickness(style.getOutlineThickness()); 
    background_.setPosition(getGlobalPosition());
    background_.setSize(Component::getSize());

    cursor_.setFillColor(sf::Color::Black);
    cursor_.setOutlineColor(sf::Color::Black);
    cursor_.setSize(sf::Vector2f{CURSOR_WIDTH, (float)style.getFontSize()+5.f});
    cursor_.setPosition(getGlobalPosition());
}

std::string EditBox::getText()
{
    return text_.getText();
}

void EditBox::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
    if (isFocused()) renderTexture.draw(cursor_);
    renderTexture.draw(text_);
}

void EditBox::onSizeChange()
{
    background_.setSize(Component::getSize());
    text_.setSize(Component::getSize());
    updateTextVisbleArea();
}

void EditBox::onPositionChange()
{
    text_.setGlobalPosition(Component::getGlobalPosition());
    background_.setPosition(getGlobalPosition());
    updateCursor();
}

void EditBox::updateCursor()
{
    auto cursor_position = cursor_.getPosition();
    cursor_position.y = text_.getGlobalPosition().y;
    cursor_position.x = text_.getGlobalPosition().x + text_.getTextWidth();

    float cursor_position_x_offset_to_end = text_.getGlobalPosition().x + text_.getSize().x - cursor_position.x; 

    if (cursor_position_x_offset_to_end < 0) 
    {
        cursor_position.x = text_.getGlobalPosition().x + text_.getSize().x - CURSOR_WIDTH;
    }
    cursor_.setPosition(cursor_position);
}

void EditBox::updateTextVisbleArea()
{
    float text_x_offset = text_.getSize().x - text_.getTextWidth();

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
            if (new_text.empty()) 
            {
                return gui::EventStatus::NotConsumed;
            }

            new_text.pop_back();
            break;
        }
        case sf::Keyboard::Return : 
        {
            defocus();
            return gui::EventStatus::Consumed;
        }
        default: break;
    }

    text_.setText(new_text);
    updateCursor();
    updateTextVisbleArea();
    return gui::EventStatus::Consumed;
}

EventStatus EditBox::on(const event::TextEntered& textEntered)
{
    if(not isFocused()) return gui::EventStatus::NotConsumed;
    std::string new_text = text_.getText();

    // FIXME: crude unicode conversion and backspace ignore (0x8) 
    if (textEntered.unicode < 128 && textEntered.unicode != 0x8)
    {
        new_text += static_cast<char>(textEntered.unicode);
    }

    text_.setText(new_text);
    updateCursor();
    updateTextVisbleArea();
    return gui::EventStatus::Consumed;  
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