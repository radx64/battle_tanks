#include <gui/EditBox.hpp>

#include "gui/StyleSheet.hpp"

#include "gui/Debug.hpp"

#include "engine/input/KeyboardUtils.hpp"

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
    text_.setPosition(Component::getGlobalPosition());

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
    return text_.getString();
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
}

void EditBox::onPositionChange()
{
    text_.setPosition(Component::getGlobalPosition());
    background_.setPosition(getGlobalPosition());
    cursor_.setPosition(getGlobalPosition());  // FIXME: this set position is messing up cursor while moving

}

EventStatus EditBox::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    if (isInside(mouseButtonPressedEvent.position)) focus();

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

    std::string new_text = text_.getString();

    if (keyboardKeyPressed.key == sf::Keyboard::BackSpace)
    {
        if (new_text.empty()) return gui::EventStatus::NotConsumed;
        new_text.pop_back();
    }
    else if (keyboardKeyPressed.key == sf::Keyboard::Return)
    {
        defocus();
        return gui::EventStatus::Consumed;
    }
    else
    {
        new_text += engine::input::keyToString(keyboardKeyPressed.key).data();
    }

    text_.setString(new_text);

    auto position = cursor_.getPosition();
    position.y = text_.getPosition().y;
    position.x = text_.getPosition().x + text_.getLocalBounds().width;
    cursor_.setPosition(position);

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