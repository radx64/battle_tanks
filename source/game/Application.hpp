#ifndef GAME_APPLICATION_HPP_
#define GAME_APPLICATION_HPP_

#include <memory>

#include <SFML/Graphics.hpp>

#include "engine/Camera.hpp"
#include "engine/CollisionSolver.hpp"
#include "engine/FpsCounter.hpp"
#include "engine/FpsLimiter.hpp"
#include "engine/ParticleSystem.hpp"
#include "engine/Renderer.hpp"
#include "engine/RigidBody.hpp"
#include "engine/Scene.hpp"

#include "game/Context.hpp"
#include "game/entity/tank/Tank.hpp"
#include "game/entity/TracksRenderer.hpp"
#include "game/Navigator.hpp"

#include "graphics/Tilemap.hpp"

#include "gui/Label.hpp"
#include "gui/Window.hpp"
#include "gui/WindowManager.hpp"

namespace game 
{

class Application
{
public:
    Application();
    int run();
    
protected:
    void configureGUI();
    void spawnSomeTanks();
    void spawnSomeBarrelsAndCratesAndTress();
    void renderGameObjects();

    engine::ParticleSystem particleSystem_;
    Context context_;
    engine::FpsCounter fpsCounter_;
    engine::FpsLimiter fpsLimiter_;

    const sf::Vector2f camera_initial_position_;
    const sf::Vector2f camera_initial_size_;
    engine::Camera camera_;
    sf::View camera_view_;

    std::unique_ptr<graphics::Tilemap> tilemap_;

    bool floating_button_demo_visible_{false};

    bool was_last_event_left_click_{true}; // temporary workaround for dumb mouse event polling
    sf::Vector2f last_mouse_in_gui_position_{};

    bool rigid_body_debug_{false};
    float timeStep_{1.0f/30.f};

    gui::Label* measurements_text_handle_;
    gui::Label* measurements_average_text_handle_;
    gui::Button* test_floating_button_handle_;
    sf::RenderWindow window_;
    std::unique_ptr<gui::WindowManager> window_manager_;

    std::vector<sf::Vector2i> waypoints_;
    std::vector<std::unique_ptr<Navigator>> navigators_;
    engine::Scene scene_;
    engine::CollisionSolver collision_solver_;
    entity::TracksRenderer tracksRenderer_;
};

}  // namespace game

#endif  // GAME_APPLICATION_HPP_