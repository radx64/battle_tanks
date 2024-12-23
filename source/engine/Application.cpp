#include <iostream>

#include "engine/Application.hpp"

#include "Config.hpp"

namespace engine
{

Application::Application(const std::string_view& windowName)
: window_(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, 32), windowName.data())
, mouseHandler_{&timerService_}
, realTimeStep_{}
, collisionSolver_(scene_)
{
    window_.setKeyRepeatEnabled(false);
    window_.setPosition(sf::Vector2i{0, 0});

    context_.setScene(&scene_);
    context_.setParticleSystem(&particleSystem_);
    context_.setTimerService(&timerService_);
}

Application::~Application() = default;

void Application::init()
{
    onInit();
}

int Application::run()
{
    try
    {
        while (isRunning_)
        {
            realTimeStep_ = std::chrono::milliseconds(clock_.restart().asMilliseconds());
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

    onClose();

    return 0;
}
void Application::close()
{
    isRunning_ = false;
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
                keyboardHandler_.handleKeyPressed(event.key);
                break;
            }
            case sf::Event::KeyReleased :
            {
                keyboardHandler_.handleKeyReleased(event.key);
                break;
            }

            case sf::Event::TextEntered :
            {
                keyboardHandler_.handleTextEntered(event.text);
                break;
            }

            case sf::Event::MouseButtonPressed :
            {
                mouseHandler_.handleButtonPressed(event.mouseButton);
                break;
            }

            case sf::Event::MouseButtonReleased :
            {
                mouseHandler_.handleButtonReleased(event.mouseButton);
                break;
            }

            case sf::Event::MouseMoved :
            {
                mouseHandler_.handleMouseMoved(event.mouseMove);
                break;
            }

            default : {}
        }
    onEvent(event);
    }
}

void Application::update()
{
    timerService_.update(realTimeStep_);
    scene_.update();
    particleSystem_.update(constantTimeStep_);
    onUpdate(constantTimeStep_);
}

void Application::render()
{
    window_.clear(sf::Color(0, 0, 0));
    onRender();
    window_.display();
}

}  // namespace engine
