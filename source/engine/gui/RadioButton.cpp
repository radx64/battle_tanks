#include "engine/gui/RadioButton.hpp"

#include "engine/gui/TextureLibrary.hpp"

#include "engine/gui/RadioButtonGroup.hpp"

namespace engine::gui
{

RadioButton::RadioButton(const bool isChecked)
    : Checkbox{isChecked,
        TextureLibrary::instance().get("radiobutton_normal"),
        TextureLibrary::instance().get("radiobutton_hover"),
        TextureLibrary::instance().get("radiobutton_focus"),
        TextureLibrary::instance().get("radiobutton_checked")}
{

}

RadioButton::~RadioButton()
{
    if (groupHandler_)
    {
        groupHandler_->remove(this);
    }
}

std::unique_ptr<RadioButton> RadioButton::create(const bool isChecked)
{
    return std::unique_ptr<RadioButton>{new RadioButton{isChecked}};
}

EventStatus RadioButton::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{    
    if (not isFocused()) return EventStatus::NotConsumed;

    auto& key = keyboardKeyReleasedEvent.key;

    if (key == engine::gui::event::Key::Space || key == engine::gui::event::Key::Enter)
    {
        setState(true);
        if (groupHandler_ && isChecked())
        {
            groupHandler_->deactivateOthers(this);
        }
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;

}

EventStatus RadioButton::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (mouseButtonReleasedEvent.button != engine::gui::event::MouseButton::Left)
    {
        return EventStatus::NotConsumed;
    }

    setState(true);
    if (groupHandler_ && isChecked())
    {
        groupHandler_->deactivateOthers(this);
    }
    return EventStatus::Consumed;
}

void RadioButton::connectGroupHandler(std::shared_ptr<RadioButtonGroup> handler)
{
    groupHandler_ = handler;
}


}  // namespace engine::gui
