#include "engine/Application.hpp"

#include <fmt/format.h>

#include "Config.hpp"
#include "engine/Logger.hpp"
#include "LoggerSink.hpp"

constexpr unsigned int ANTI_ALIASING_LEVEL = 4;

namespace engine
{

Application::Application(const std::string_view windowName, const std::string_view logPrefix, const sf::Vector2f& windowSize)
: window_(sf::VideoMode(windowSize.x, windowSize.y, 32), windowName.data(),
    sf::Style::Default, sf::ContextSettings(0, 0, ANTI_ALIASING_LEVEL))
, profilingText_{"", font_, 16}
, mouseHandler_{&timerService_}
, realTimeStep_{}
, collisionSolver_{scene_}
, logger_{logPrefix}
, eventsProfiler_{"engine::Application::process_events", "ms"}
, updateProfiler_{"engine::Application::update", "ms"}
, renderProfiler_{"engine::Application::render", "ms"}
{
    window_.setKeyRepeatEnabled(false);
    window_.setPosition(sf::Vector2i{0, 0});

    context_.setScene(&scene_);
    context_.setParticleSystem(&particleSystem_);
    context_.setTimerService(&timerService_);

    font_.loadFromFile("./resources/fonts/Px437_IBM_VGA_8x16.ttf");
    profilingText_.setPosition({10.f, 10.f});
    profilingText_.setFillColor(sf::Color::Black);
    profilingText_.setOutlineColor(sf::Color::White);
    profilingText_.setOutlineThickness(2.f);
    profilingText_.setCharacterSize(16);
}

Application::~Application()
{
}

void Application::init()
{
    try
    {
        logger_.debug("Initializing application.");
        onInit();
    }
    catch (const std::exception& e)
    {
        logger_.error(fmt::format("Exception was thrown: {}!",  e.what()));
        exit(-1);
    }
    catch(...)
    {
        logger_.error("Unknown exception was thrown!");
    }
    
}

int Application::run()
{
    logger_.debug("Starting application.");
    try
    {
        while (isRunning_)
        {
            profiling_.reset();
            realTimeStep_ = std::chrono::milliseconds(clock_.restart().asMilliseconds());
            PROFILE_SCOPE(eventsProfiler_,
                processEvents();
            );

            PROFILE_SCOPE(updateProfiler_,
                update();
            );
 
            render();
        }
    }
    catch (const std::exception& e)
    {
        logger_.error(fmt::format("EXCEPTION THROWN: {}",  e.what()));
        return -1;
    }

    onClose();

    return 0;
}
void Application::close()
{
    logger_.debug("Closing application.");
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
    {
        PROFILE_SCOPE(renderProfiler_,
            window_.clear(sf::Color(0, 0, 0));
            onRender();   
        ); 
    }
    renderProfilingInfo();
    window_.display();
}

void Application::renderProfilingInfo()
{    
    std::string text;
    for (const auto& result : profiling_.get())
    {
        text += fmt::format("{}({:.3f}|{:.3f}){}\n", result.name, result.average, result.lastFrame, result.unit);
    }
    
    profilingText_.setString(text);
    window_.draw(profilingText_);
}

}  // namespace engine
