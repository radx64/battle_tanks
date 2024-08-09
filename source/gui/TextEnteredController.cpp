#include "gui/TextEnteredController.hpp"

#include "gui/WindowManager.hpp"

namespace gui
{

TextEnteredController::TextEnteredController(gui::WindowManager* window_manager)
: window_manager_(window_manager)
{}

void TextEnteredController::onTextEntered(const uint32_t unicode)
{
    window_manager_->receive(gui::event::TextEntered{.unicode = unicode});
}

}  // namespace gui