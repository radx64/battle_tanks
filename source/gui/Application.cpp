#include "Application.hpp"

#include "gui/FontLibrary.hpp"
#include "gui/TextureLibrary.hpp"

namespace gui
{
Application::Application(const std::string_view windowName, const std::string_view logPrefix, const sf::Vector2f& windowSize)
: engine::Application{windowName, logPrefix, windowSize}
, windowManager_{windowSize}
, mouseController_{&windowManager_, window_, window_.getDefaultView()}
, keyboardController_{&windowManager_}
, textEnteredController_{&windowManager_}
{
    gui::FontLibrary::init();
    gui::TextureLibrary::init();

    mouseHandler_.subscribe(&mouseController_);
    keyboardHandler_.subscribe(&keyboardController_);
    keyboardHandler_.subscribe(&textEnteredController_);
}

Application::~Application()
{
    gui::FontLibrary::destroy();
}

void Application::onRender()
{
    windowManager_.render(window_);
}

}  // namespace gui
