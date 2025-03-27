#pragma once

#include "engine/Application.hpp"
#include "engine/Timer.hpp"

#include "gui/WindowManager.hpp"
#include "gui/KeyboardController.hpp"
#include "gui/MouseController.hpp"
#include "gui/TextEnteredController.hpp"
#include "gui/FramedSprite.hpp"

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

    void generateBackground();

    gui::WindowManager windowManager_;
    gui::MouseController mouseController_;
    gui::KeyboardController keyboardController_;
    gui::TextEnteredController textEnteredController_;

    sf::RenderTexture backgroundTexture_;
    sf::Sprite backgroundSprite_;
    std::unique_ptr<gui::FramedSprite> framedSpriteTest_;
    std::unique_ptr<gui::FramedSprite> framedSpriteTest2_;
    engine::Timer timer_;
};

}  // namespace gui_sandbox
