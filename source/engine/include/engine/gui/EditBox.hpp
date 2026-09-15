#pragma once

#include <memory>

#include "engine/gui/BaseEditBox.hpp"

namespace engine::gui
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
    EventStatus on(const engine::events::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const engine::events::MouseMoved& mouseMovedEvent) override;
    EventStatus on(const engine::events::KeyPressed& keyboardKeyPressed) override;
};

}  // namespace engine::gui
