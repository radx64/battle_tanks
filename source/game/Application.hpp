#pragma once

#include <memory>

#include "engine/Camera.hpp"
#include "engine/CameraController.hpp"
#include "engine/FpsCounter.hpp"
#include "engine/math/Math.hpp"
#include "engine/Profiler.hpp"
#include "engine/Renderer.hpp"
#include "engine/RigidBody.hpp"

#include "game/entity/tank/Tank.hpp"
#include "game/entity/tank/TankController.hpp"
#include "game/entity/TracksRenderer.hpp"
#include "game/WaypointMouseController.hpp"

#include "graphics/Tilemap.hpp"

#include "gui/Application.hpp"
#include "gui/Label.hpp"
#include "gui/Window.hpp"
#include "gui/WindowManager.hpp"
#include "gui/MouseController.hpp"

#include "lua/ScriptsScheduler.hpp"

namespace game 
{

class Application : public gui::Application
{
public:
    Application();
    
protected:

    void onInit() override;
    void onClose() override;
    void onEvent(const sf::Event& event) override;
    void onUpdate(float time_step) override;
    void onRender() override;

    void configureGUI();
    void spawnSomeTanks();
    void spawnSomeBarrelsAndCratesAndTress();
    void renderGameObjects();
    void generateProfiling();

    engine::FpsCounter fps_counter_;

    const sf::Vector2f camera_initial_size_;
    const sf::Vector2f camera_world_size_;
    const sf::Vector2f camera_initial_position_;
    engine::Camera camera_;
    engine::CameraController camera_controller_;
    sf::View camera_view_;

    std::unique_ptr<graphics::Tilemap> tilemap_;

    bool rigid_body_debug_{false};
    bool tank_debug_mode_{false};
    
    game::WaypointMouseController waypoint_mouse_controller_;

    std::vector<sf::Vector2i> waypoints_;
    std::vector<std::unique_ptr<game::entity::TankController>> lua_controllers_;
    lua::ScriptsScheduler scripts_scheduler_;

    entity::TracksRenderer tracks_renderer_;
    engine::math::Average<float> fps_average_;

    engine::Profiler draw_profiler_;
    engine::Profiler physics_profiler_;
    engine::Profiler gui_profiler_;
    engine::Profiler lua_profiler_;
    engine::Profiler tilemap_profiler_;
    engine::Profiler render_game_objects_profiler_;
    engine::Profiler tracks_profiler_;
    engine::Profiler waypoints_profiler_;
    engine::Profiler particles_profiler_;
    engine::Profiler display_profiler_;
};

}  // namespace game
