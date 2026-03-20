#include "gui/TextEnteredController.hpp"

#include "gui/EventReceiver.hpp"

namespace gui
{

TextEnteredController::TextEnteredController(gui::EventReceiver* receiver)
: receiver_(receiver)
{}

void TextEnteredController::onTextEntered(const uint32_t unicode)
{
    receiver_->receive(gui::event::TextEntered{.unicode = unicode});
}

}  // namespace gui
