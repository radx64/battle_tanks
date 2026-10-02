#pragma once

#include <vector>

#include "engine/Window.hpp"
#include "engine/gfx/View.hpp"
#include "engine/input/MouseReceiver.hpp"

namespace game
{

class WaypointMouseController : public engine::input::MouseReceiver
{
public:
    WaypointMouseController(std::vector<engine::Vec2i>& waypoints, engine::Window& window, engine::gfx::View& view);

protected:
    engine::gui::EventStatus onButtonPressed(const engine::Vec2f& mouse_postion, const engine::core::MouseButton& button, bool double_click) override;
    engine::gui::EventStatus onButtonReleased(const engine::Vec2f& mouse_postion, const engine::core::MouseButton& button) override;
    engine::gui::EventStatus onMouseMoved(const engine::Vec2f& mouse_postion) override;
    engine::gui::EventStatus onMouseScrolled(const engine::Vec2f& mouse_position, const engine::core::MouseWheel& wheel, const float delta) override;

    engine::Vec2f mapPixelToCoords(const engine::Vec2f& mouse_position);

    std::vector<engine::Vec2i>& waypoints_;
    engine::Window& window_;
    engine::gfx::View& view_;
};


}  // namespace game
