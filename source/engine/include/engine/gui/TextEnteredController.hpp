#pragma once

#include <engine/input/TextEnteredReceiver.hpp>

namespace engine::gui {class EventReceiver;}

namespace engine::gui
{
class TextEnteredController : public engine::input::TextEnteredReceiver
{
public:
    TextEnteredController(engine::gui::EventReceiver* receiver);

protected:
    void onTextEntered(const uint32_t unicode) override;

    engine::gui::EventReceiver* receiver_;
};

}  // namespace engine::gui
