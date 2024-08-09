#include <iostream>

#include "engine/Application.hpp"

#include "Config.hpp"

namespace engine
{

Application::Application(const std::string_view& windowName)
: window_(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, 32), windowName.data())
,collision_solver_(scene_)
{
    window_.setKeyRepeatEnabled(false);
    window_.setPosition(sf::Vector2i{0, 0});
}

void Application::init()
{
    onInit();
}

int Application::run()
{
    try
    {
        while (is_running_)
        {
            clock_.restart();
            processEvents();
            update();
            render();
        }
    }
    catch (std::exception& e)
    {
        std::cout << "EXCEPTION THROWN: " << e.what() << std::endl;
        return -1;
    }

    window_.close();

    return 0;
}
void Application::close()
{
    is_running_ = false;
    onClose();
    window_.close();
}

void Application::processEvents()
{
    sf::Event event;
    while (window_.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed : { close(); break; }
            case sf::Event::KeyPressed :
            {
                keyboard_handler_.handleKeyPressed(event.key);
                break;
            } 
            case sf::Event::KeyReleased : 
            {
                keyboard_handler_.handleKeyReleased(event.key);
                break;
            }

            case sf::Event::TextEntered : 
            {
                keyboard_handler_.handleTextEntered(event.text);
                break;
            }

            case sf::Event::MouseButtonPressed :
            {
                mouse_handler_.handleButtonPressed(event.mouseButton);
                break;
            }

            case sf::Event::MouseButtonReleased :
            {
                mouse_handler_.handleButtonReleased(event.mouseButton);
                break;
            }

            case sf::Event::MouseMoved :
            {
                mouse_handler_.handleMouseMoved(event.mouseMove);
                break;
            }

            default : {}
        }
    onEvent(event);
    }
}

void Application::update()
{
    scene_.update();
    particle_system_.update(timeStep_);
    onUpdate(timeStep_);
}

void Application::render()
{
    window_.clear(sf::Color(0, 0, 0));
    onRender();
    window_.display();
}

}  // namespace engine