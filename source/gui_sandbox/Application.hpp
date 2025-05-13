#pragma once

#include "engine/Timer.hpp"

#include "gui/Application.hpp"
#include "gui/WindowManager.hpp"
#include "gui/KeyboardController.hpp"
#include "gui/MouseController.hpp"
#include "gui/TextEnteredController.hpp"

namespace gui_sandbox
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

    void generateBackground();

    sf::RenderTexture backgroundTexture_;
    sf::Sprite backgroundSprite_;
    engine::Timer timer_;
};

}  // namespace gui_sandbox
