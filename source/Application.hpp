#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include <memory>

#include <SFML/Graphics.hpp>

#include "Camera.hpp"
#include "Context.hpp"
#include "Navigator.hpp"
#include "Particles.hpp"
#include "Tank.hpp"
#include "Tilemap.hpp"

class Application
{
public:
    Application();
    int run();
    
protected:
    void configureTexts();
    void spawnSomeTanks();

    Particles particles_;
    Context context_;

    const sf::Vector2f camera_initial_position_;
    const sf::Vector2f camera_initial_size_;
    Camera camera_;
    sf::View view_;

    std::unique_ptr<Tilemap> tilemap_;

    sf::Text help_text_;
    sf::Text measurements_text_;
    sf::Text measurements_average_text_;

    std::vector<sf::Vector2i> waypoints_;
    std::vector<std::unique_ptr<Tank>> tanks_;
    std::vector<std::unique_ptr<Navigator>> navigators_;
};

#endif  // APPLICATION_HPP_