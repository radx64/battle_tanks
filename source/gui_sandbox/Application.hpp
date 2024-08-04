#pragma once

#include "engine/Application.hpp"
#include "gui/WindowManager.hpp"
#include "gui/MouseController.hpp"

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

    std::unique_ptr<gui::WindowManager> window_manager_;
    std::unique_ptr<gui::MouseController> mouse_controller_;
};

}  // namespace gui_sandbox



