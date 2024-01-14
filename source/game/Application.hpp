#ifndef GAME_APPLICATION_HPP_
#define GAME_APPLICATION_HPP_

#include <memory>

#include <SFML/Graphics.hpp>

#include "game/Camera.hpp"
#include "game/CollisionSolver.hpp"
#include "game/Context.hpp"
#include "game/FpsCounter.hpp"
#include "game/FpsLimiter.hpp"
#include "game/Navigator.hpp"
#include "game/Renderer.hpp"
#include "game/RigidBody.hpp"
#include "game/Tank.hpp"
#include "game/World.hpp"

#include "graphics/ParticleSystem.hpp"
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

    graphics::ParticleSystem particleSystem_;
    Context context_;
    game::FpsCounter fpsCounter_;
    game::FpsLimiter fpsLimiter_;

    const sf::Vector2f camera_initial_position_;
    const sf::Vector2f camera_initial_size_;
    Camera camera_;
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
    game::World world_;
    game::CollisionSolver collision_solver_;
};

}  // namespace game

#endif  // GAME_APPLICATION_HPP_