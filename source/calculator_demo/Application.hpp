#pragma once

#include "gui/Application.hpp"
#include "gui/KeyboardController.hpp"
#include "gui/MouseController.hpp"
#include "gui/TextEnteredController.hpp"
#include "gui/WindowManager.hpp"

namespace calculator
{
class Application : public gui::Application
{
public:
    Application();

protected:
    void onInit() override;
    void onClose() override;
    void onEvent(const sf::Event& event) override;
    void onUpdate(float timeStep) override;
    void onRender() override;
};

}  // namespace calculator
