#pragma once

#include "engine/Application.hpp"

#include "engine/gui/GUI.hpp"
#include "engine/gui/KeyboardController.hpp"
#include "engine/gui/MouseController.hpp"
#include "engine/gui/TextEnteredController.hpp"
#include "engine/gui/WindowManager.hpp"

namespace engine::gui
{
class Application : public engine::Application
{
public:
    Application(const std::string_view windowName, const std::string_view logPrefix, const sf::Vector2f& windowSize);
    ~Application();

protected:
    void onRender() override; 
    void onUpdate(float time_step) override;
    engine::gui::GUI& gui();

    engine::gui::GUI gui_;
    engine::gui::MouseController mouseController_;
    engine::gui::KeyboardController keyboardController_;
    engine::gui::TextEnteredController textEnteredController_;
};

}  // namespace engine::gui
