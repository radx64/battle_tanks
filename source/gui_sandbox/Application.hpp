#pragma once

#include "engine/Timer.hpp"

#include "engine/gui/Application.hpp"
#include "engine/gui/WindowManager.hpp"
#include "engine/gui/KeyboardController.hpp"
#include "engine/gui/MouseController.hpp"
#include "engine/gui/TextEnteredController.hpp"

namespace gui_sandbox
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

    void generateBackground();

    sf::RenderTexture backgroundTexture_;
    sf::Sprite backgroundSprite_;
    engine::Timer timer_;

    bool isSyncing_ = false;
};

}  // namespace gui_sandbox
