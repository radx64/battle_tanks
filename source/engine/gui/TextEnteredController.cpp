#include "engine/gui/TextEnteredController.hpp"

#include "engine/gui/EventReceiver.hpp"

namespace engine::gui
{

TextEnteredController::TextEnteredController(engine::gui::EventReceiver* receiver)
: receiver_(receiver)
{}

void TextEnteredController::onTextEntered(const uint32_t unicode)
{
    receiver_->receive(engine::gui::event::TextEntered{.unicode = unicode});
}

}  // namespace engine::gui
