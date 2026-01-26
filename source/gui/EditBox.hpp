#pragma once

#include <memory>

#include "gui/BaseEditBox.hpp"

namespace gui
{

class EditBox : public BaseEditBox
{
public:
    static std::unique_ptr<EditBox> create();

    ~EditBox() = default;

protected:
    EditBox();

    void updateTextVisbleArea() override;

private:
    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;
    EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressed) override;
};

}  // namespace gui
