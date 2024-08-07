#pragma once
#include "engine/Application.hpp"
#include "gui/WindowManager.hpp"
#include "gui/KeyboardController.hpp"
#include "gui/MouseController.hpp"
#include "gui/TextEnteredController.hpp"

namespace gui_sandbox
{
class Application : public engine::Application
{
public:
    Application();

protected:
    void onInit() override;
    void onClose() override;
    void onEvent(const sf::Event& event) override;
    void onUpdate(float timeStep) override;
    void onRender() override; 

    gui::WindowManager window_manager_;
    gui::MouseController mouse_controller_;
    gui::KeyboardController keyboard_controller_;
    gui::TextEnteredController text_entered_controller_;
};

}  // namespace gui_sandbox



