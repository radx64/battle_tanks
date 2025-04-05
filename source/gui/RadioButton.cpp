#include "gui/RadioButton.hpp"

#include "gui/TextureLibrary.hpp"

namespace gui
{

RadioButton::RadioButton(const bool isChecked)
    : Checkbox{isChecked,
        TextureLibrary::instance().get("radiobutton_normal"),
        TextureLibrary::instance().get("radiobutton_hover"),
        TextureLibrary::instance().get("radiobutton_focus"),
        TextureLibrary::instance().get("radiobutton_checked")}
{

}

std::unique_ptr<RadioButton> RadioButton::create(const bool isChecked)
{
    return std::unique_ptr<RadioButton>{new RadioButton{isChecked}};
}

EventStatus RadioButton::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{    
    if (not isFocused()) return EventStatus::NotConsumed;

    auto& key = keyboardKeyReleasedEvent.key;

    if (key == gui::event::Key::Space || key == gui::event::Key::Enter)
    {
        isChecked_ = !isChecked_;
        updateTexture();
        
        if (onClick_) onClick_(isChecked());
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;

}

EventStatus RadioButton::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
   // TODO: implement support of radiogroup so mouse and keyboard handling will have to be overridden
    (void)mouseButtonReleasedEvent;
    return EventStatus::NotConsumed;
}

}  // namespace gui
