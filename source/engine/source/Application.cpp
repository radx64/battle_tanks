#include "engine/Application.hpp"

#include <functional>

#include <fmt/format.h>

#include "engine/Logger.hpp"
#include "engine/events/ApplicationEvent.hpp"
#include "engine/events/Event.hpp"
#include "engine/events/EventDispatcher.hpp"
#include "engine/events/KeyEvent.hpp"
#include "engine/events/MouseEvent.hpp"

constexpr unsigned int ANTI_ALIASING_LEVEL = 4;

namespace engine
{

Application::Application(const std::string_view windowName, const std::string_view logPrefix, const engine::Vec2f& windowSize)
: window_(engine::WindowProperties{windowName.data(), {static_cast<int>(windowSize.x), static_cast<int>(windowSize.y)}})
, profilingText_{"", font_, 16}
, mouseHandler_{&timer_service_}
, realtime_step_{}
, collisionSolver_{scene_}
, logger_{logPrefix}
, eventsProfiler_{"engine::Application::process_events", "ms"}
, updateProfiler_{"engine::Application::update", "ms"}
, renderProfiler_{"engine::Application::render", "ms"}
, clearProfiler_{"engine::window.clear", "ms"}
, renderProfilingInfoProfiler_{"engine::renderProfilingInfo", "ms"}
, display_profiler_{"engine::window.display", "ms"}
{
    window_.setKeyRepeatEnabled(false);
    window_.setPosition(engine::Vector2i{0, 0});

    context_.setScene(&scene_);
    context_.setParticleSystem(&particle_system_);
    context_.setTimerService(&timer_service_);

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
    window_.setEventsCallback([this](auto& event) {processEvent(event);});

    try
    {
        while (isRunning_)
        {
            profiling_.reset();
            realtime_step_ = std::chrono::milliseconds(clock_.restart().asMilliseconds());
            PROFILE_SCOPE(eventsProfiler_,
                window_.pollEvents();
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

void Application::processEvent(engine::events::Event& event)
{
    events::EventDispatcher dispatcher(event);

    bool was_consumed {false};

    was_consumed |= dispatcher.dispatch<events::WindowClosedEvent>([this](const auto&) {close(); return true;});

    was_consumed |= dispatcher.dispatch<events::KeyPressed>([this] (const auto& event)
    {
        keyboardHandler_.handleKeyPressed(event); return false;
    });

    was_consumed |= dispatcher.dispatch<events::KeyReleased>([this] (const auto& event)
    {
        keyboardHandler_.handleKeyReleased(event); return false;
    });

    was_consumed |= dispatcher.dispatch<events::TextEntered>([this] (const auto& event)
    {
        keyboardHandler_.handleTextEntered(event); return false;
    });

    was_consumed |= dispatcher.dispatch<events::MouseButtonPressed>([this] (const auto& event)
    {
        mouseHandler_.handleButtonPressed(event); return false;
    });

    was_consumed |= dispatcher.dispatch<events::MouseButtonReleased>([this] (const auto& event)
    {
        mouseHandler_.handleButtonReleased(event); return false;
    });

    was_consumed |= dispatcher.dispatch<events::MouseMoved>([this] (const auto& event)
    {
        mouseHandler_.handleMouseMoved(event); return false;
    });

    was_consumed |= dispatcher.dispatch<events::MouseScrolled>([this] (const auto& event)
    {
        mouseHandler_.handleMouseScrolled(event); return false;
    });

    if (not was_consumed)
    {
        // Forward event to application specific handling
        onEvent(event);
    }
}


// void Application::processEvents()
// {


    // sf::Event event;
    // while (window_.getNativeHandle().pollEvent(event))
    // {
    //     switch (event.type)
    //     {
    //         case sf::Event::Closed : { close(); break; }
    //         case sf::Event::KeyPressed :
    //         {
    //             keyboardHandler_.handleKeyPressed(event.key);
    //             break;
    //         }
    //         case sf::Event::KeyReleased :
    //         {
    //             keyboardHandler_.handleKeyReleased(event.key);
    //             break;
    //         }

    //         case engine::events::TextEnteredEvent :
    //         {
    //             keyboardHandler_.handleTextEntered(event.text);
    //             break;
    //         }

    //         case sf::engine::events::MouseButtonPressed :
    //         {
    //             mouseHandler_.handleButtonPressed(event.mouseButton);
    //             break;
    //         }

    //         case sf::engine::events::MouseButtonReleased :
    //         {
    //             mouseHandler_.handleButtonReleased(event.mouseButton);
    //             break;
    //         }

    //         case sf::Event::MouseMoved :
    //         {
    //             mouseHandler_.handleMouseMoved(event.mouseMove);
    //             break;
    //         }

    //         default : {}
    //     }
    // onEvent(event);
    // }
//}

void Application::update()
{
    
    timer_service_.update(realtime_step_);
    scene_.update();
    particle_system_.update(constanttime_step_);
    onUpdate(constanttime_step_);
}

void Application::render()
{
    PROFILE_SCOPE(clearProfiler_,
        window_.getNativeHandle().clear(sf::Color(0, 0, 0));
    );
    
    PROFILE_SCOPE(renderProfiler_,
        onRender();   
    );
    
    PROFILE_SCOPE(renderProfilingInfoProfiler_,
        renderProfilingInfo();
    );
    
    PROFILE_SCOPE(display_profiler_,
        window_.getNativeHandle().display();
    );
}

void Application::renderProfilingInfo()
{    
    std::string text;
    for (const auto& result : profiling_.get())
    {
        text += fmt::format("{}({:.3f}|{:.3f}){}\n", result.name, result.average, result.lastFrame, result.unit);
    }
    
    profilingText_.setString(text);
    window_.getNativeHandle().draw(profilingText_);
}

}  // namespace engine
