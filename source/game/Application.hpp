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

    sf::Text help_text_;
    bool help_visible_{false};
    bool label_demo_visible_{false};

    gui::Label* measurements_text_handle_;
    gui::Label* measurements_average_text_handle_;
    gui::Window* help_window_handle_;

    std::vector<sf::Vector2i> waypoints_;
    std::vector<std::unique_ptr<Tank>> tanks_;
    std::vector<std::unique_ptr<Navigator>> navigators_;

    std::vector<std::unique_ptr<gui::Component>> guiElements_;
};

}  // namespace game

#endif  // GAME_APPLICATION_HPP_