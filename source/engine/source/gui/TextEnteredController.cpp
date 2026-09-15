#include "engine/gui/TextEnteredController.hpp"

#include "engine/gui/EventReceiver.hpp"

namespace engine::gui
{

TextEnteredController::TextEnteredController(engine::gui::EventReceiver* receiver)
: receiver_(receiver)
{}

void TextEnteredController::onTextEntered(const engine::events::TextEntered& event)
{
    receiver_->receive(event);
}

}  // namespace engine::gui
