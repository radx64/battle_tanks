#include <gui/EditBox.hpp>

#include "gui/StyleSheet.hpp"

#include "gui/Debug.hpp"

#include "engine/input/KeyboardUtils.hpp"

constexpr float CURSOR_WIDTH = 5.f;

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

    background_.setFillColor(sf::Color::White);
    background_.setOutlineColor(style.getOutlineColor());
    background_.setOutlineThickness(style.getOutlineThickness()); 
    background_.setPosition(getGlobalPosition());
    background_.setSize(Component::getSize());

    cursor_.setFillColor(sf::Color::Black);
    cursor_.setOutlineColor(sf::Color::Black);
    cursor_.setSize(sf::Vector2f{CURSOR_WIDTH, (float)style.getFontSize()+10.f});
    cursor_.setPosition(getGlobalPosition());
}

void EditBox::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
    renderTexture.draw(cursor_);
    renderTexture.draw(text_);
    debug::draw_bounds(renderTexture, this);
}

void EditBox::onSizeChange()
{
    background_.setSize(Component::getSize());
}

void EditBox::onPositionChange()
{
    text_.setPosition(Component::getGlobalPosition());
    background_.setPosition(getGlobalPosition());
    cursor_.setPosition(getGlobalPosition());
}

EventStatus EditBox::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    UNUSED(mouseButtonPressedEvent);
    return gui::EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    UNUSED(mouseButtonReleasedEvent);
    return gui::EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::KeyboardKeyPressed& keyboardKeyPressed)
{
    background_.setFillColor(sf::Color::Magenta);
     std::string new_text = text_.getString();

    if (keyboardKeyPressed.key == sf::Keyboard::Space)
    {
        new_text += " ";

    } else if (keyboardKeyPressed.key == sf::Keyboard::BackSpace)
    {
        if (new_text.empty()) return gui::EventStatus::NotConsumed;;
        new_text.pop_back();
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
    background_.setFillColor(sf::Color::White);
    return gui::EventStatus::Consumed;
}

}  // namespace gui