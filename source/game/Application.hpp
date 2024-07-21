#pragma once


#include <memory>

#include "engine/Application.hpp"
#include "engine/Camera.hpp"
#include "engine/CameraController.hpp"

#include "engine/FpsCounter.hpp"

#include "engine/Renderer.hpp"
#include "engine/RigidBody.hpp"

#include "engine/math/Math.hpp"

#include "game/Context.hpp"
#include "game/entity/tank/Tank.hpp"
#include "game/entity/TracksRenderer.hpp"
#include "game/MouseController.hpp"
#include "game/Navigator.hpp"

#include "graphics/Tilemap.hpp"

#include "gui/Label.hpp"
#include "gui/Window.hpp"
#include "gui/WindowManager.hpp"

namespace game 
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

    void configureGUI();
    void spawnSomeTanks();
    void spawnSomeBarrelsAndCratesAndTress();
    void renderGameObjects();
    void generateProfiling();

    Context context_;
   
    engine::FpsCounter fpsCounter_;

    const sf::Vector2f camera_initial_position_;
    const sf::Vector2f camera_initial_size_;
    engine::Camera camera_;
    engine::CameraController camera_controller_;
    sf::View camera_view_;

    std::unique_ptr<graphics::Tilemap> tilemap_;

    bool rigid_body_debug_{false};
    bool tank_debug_mode_{false};
    
    gui::Label* measurements_text_handle_;
    gui::Label* measurements_average_text_handle_;

    std::unique_ptr<gui::WindowManager> window_manager_;
    std::unique_ptr<game::MouseController> mouse_controller_;

    std::vector<sf::Vector2i> waypoints_;
    std::vector<std::unique_ptr<Navigator>> navigators_;

    entity::TracksRenderer tracks_renderer_;

    sf::Int32 draw_time_{0};
    sf::Time physics_time_{};
    sf::Time nav_time_{};
    sf::Time gui_time_{};

    engine::math::Average draw_average_;
    engine::math::Average physics_average_;
    engine::math::Average nav_average_;
    engine::math::Average fps_average_;
    engine::math::Average gui_average_;
};

}  // namespace game

