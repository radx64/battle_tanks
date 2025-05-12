#pragma once

#include "engine/Application.hpp"

#include "gui/KeyboardController.hpp"
#include "gui/MouseController.hpp"
#include "gui/TextEnteredController.hpp"
#include "gui/WindowManager.hpp"

namespace gui
{
class Application : public engine::Application
{
public:
    Application(const std::string_view windowName, const std::string_view logPrefix, const sf::Vector2f& windowSize);
    ~Application();

protected:
    void onRender() override; 

    gui::WindowManager windowManager_;
    gui::MouseController mouseController_;
    gui::KeyboardController keyboardController_;
    gui::TextEnteredController textEnteredController_;
};

}  // namespace gui
