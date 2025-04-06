#pragma once

#include <memory>
#include <functional>

#include "gui/Checkbox.hpp"

namespace gui {class RadioButtonGroup;}  // namespace gui

namespace gui
{

class RadioButton : public Checkbox
{
public:
    static std::unique_ptr<RadioButton> create(const bool isChecked);
    ~RadioButton() override;

protected:
    RadioButton(const bool isChecked);


    friend RadioButtonGroup;
    void connectGroupHandler(std::shared_ptr<RadioButtonGroup> handler);
    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;

    std::shared_ptr<RadioButtonGroup> groupHandler_;
};

}  // namespace gui
