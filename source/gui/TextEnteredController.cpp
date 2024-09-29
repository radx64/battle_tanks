#include "gui/TextEnteredController.hpp"

#include "gui/WindowManager.hpp"

namespace gui
{

TextEnteredController::TextEnteredController(gui::WindowManager* windowManager)
: windowManager_(windowManager)
{}

void TextEnteredController::onTextEntered(const uint32_t unicode)
{
    windowManager_->receive(gui::event::TextEntered{.unicode = unicode});
}

}  // namespace gui
