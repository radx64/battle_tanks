#pragma once

#include <memory>
#include <functional>

#include "engine/gui/Checkbox.hpp"

namespace engine::gui {class RadioButtonGroup;}  // namespace engine::gui

namespace engine::gui
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

}  // namespace engine::gui
