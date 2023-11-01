#ifndef GAME_APPLICATION_HPP_
#define GAME_APPLICATION_HPP_

#include <memory>

#include <SFML/Graphics.hpp>

#include "game/Camera.hpp"
#include "game/Context.hpp"
#include "game/Navigator.hpp"
#include "game/Tank.hpp"
#include "graphics/Particles.hpp"
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
    void configureTexts();
    void spawnSomeTanks();

    graphics::Particles particles_;
    Context context_;

    const sf::Vector2f camera_initial_position_;
    const sf::Vector2f camera_initial_size_;
    Camera camera_;
    sf::View view_;

    std::unique_ptr<graphics::Tilemap> tilemap_;

    bool help_visible_{false};
    bool label_demo_visible_{false};
    bool was_last_event_left_click_{false}; // temporary workaround for dumb mouse event polling

    gui::Label* measurements_text_handle_;
    gui::Label* measurements_average_text_handle_;
    gui::Window* help_window_handle_;
    std::unique_ptr<gui::WindowManager> window_manager_;

    std::vector<sf::Vector2i> waypoints_;
    std::vector<std::unique_ptr<Tank>> tanks_;
    std::vector<std::unique_ptr<Navigator>> navigators_;

    std::vector<std::unique_ptr<gui::Component>> guiElements_;
};

}  // namespace game

#endif  // GAME_APPLICATION_HPP_