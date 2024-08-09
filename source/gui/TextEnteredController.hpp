#pragma once

#include <engine/input/TextEnteredReceiver.hpp>

namespace gui {class WindowManager;}

namespace gui
{
class TextEnteredController : public engine::input::TextEnteredReceiver
{
public:
    TextEnteredController(gui::WindowManager* window_manager);

protected:
    void onTextEntered(const uint32_t unicode) override;

    gui::WindowManager* window_manager_;
};

}  // namespace gui