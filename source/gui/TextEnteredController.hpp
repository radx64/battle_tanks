#pragma once

#include <engine/input/TextEnteredReceiver.hpp>

namespace gui {class EventReceiver;}

namespace gui
{
class TextEnteredController : public engine::input::TextEnteredReceiver
{
public:
    TextEnteredController(gui::EventReceiver* receiver);

protected:
    void onTextEntered(const uint32_t unicode) override;

    gui::EventReceiver* receiver_;
};

}  // namespace gui
