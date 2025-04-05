#pragma once

#include <memory>
#include <functional>

#include "gui/Checkbox.hpp"

namespace gui
{

class RadioButton : public Checkbox
{
public:
    static std::unique_ptr<RadioButton> create(const bool isChecked);

protected:
    RadioButton(const bool isChecked);
 
    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
};

}  // namespace gui
