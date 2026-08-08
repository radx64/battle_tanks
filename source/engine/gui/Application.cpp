#include "Application.hpp"

#include "engine/gui/FontLibrary.hpp"
#include "engine/gui/TextureLibrary.hpp"

namespace engine::gui
{
Application::Application(const std::string_view windowName, const std::string_view logPrefix, const sf::Vector2f& windowSize)
: engine::Application{windowName, logPrefix, windowSize}
, gui_{windowSize}
, mouseController_{&gui_, window_, window_.getDefaultView()}
, keyboardController_{&gui_}
, textEnteredController_{&gui_}
{
    engine::gui::FontLibrary::init();
    engine::gui::TextureLibrary::init();

    mouseHandler_.subscribe(&mouseController_);
    keyboardHandler_.subscribe(&keyboardController_);
    keyboardHandler_.subscribe(&textEnteredController_);
}

Application::~Application()
{
    engine::gui::FontLibrary::destroy();
}

void Application::onRender()
{
    gui().render(window_);
}

void Application::onUpdate(float time_step)
{
    (void) time_step;
    gui().update();
}

gui::GUI& Application::gui()
{
    return gui_;
}

}  // namespace engine::gui
