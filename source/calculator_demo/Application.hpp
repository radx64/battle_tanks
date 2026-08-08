#pragma once

#include "engine/gui/Application.hpp"
#include "engine/gui/KeyboardController.hpp"
#include "engine/gui/MouseController.hpp"
#include "engine/gui/TextEnteredController.hpp"
#include "engine/gui/WindowManager.hpp"

namespace calculator
{
class Application : public engine::gui::Application
{
public:
    Application();

protected:
    void onInit() override;
    void onClose() override;
    void onEvent(const sf::Event& event) override;
    void onUpdate(float time_step) override;
    void onRender() override;
};

}  // namespace calculator
