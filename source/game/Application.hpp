#ifndef GAME_CAPPLICATION_HPP_
#define GAME_CAPPLICATION_HPP_

#include <memory>

#include <SFML/Graphics.hpp>

#include "game/Camera.hpp"
#include "game/Context.hpp"
#include "game/Navigator.hpp"
#include "game/Tank.hpp"
#include "graphics/Particles.hpp"
#include "graphics/Tilemap.hpp"

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
    sf::Text measurements_text_;
    sf::Text measurements_average_text_;

    std::vector<sf::Vector2i> waypoints_;
    std::vector<std::unique_ptr<Tank>> tanks_;
    std::vector<std::unique_ptr<Navigator>> navigators_;
};

#endif  // GAME_CAPPLICATION_HPP_