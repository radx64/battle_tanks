#pragma once

#include <SFML/Graphics.hpp>

#include "engine/Clock.hpp"
#include "engine/CollisionSolver.hpp"
#include "engine/Context.hpp"
#include "engine/input/KeyboardHandler.hpp"
#include "engine/input/MouseHandler.hpp"
#include "engine/Logger.hpp"
#include "engine/ParticleSystem.hpp"
#include "engine/Scene.hpp"
#include "engine/TimerService.hpp"

namespace engine
{

class Application
{
public:
    Application(const std::string_view windowName, const std::string_view logPrefix);
    virtual ~Application();
    
    void init();
    int run();
    void close();

protected:
    void processEvents();
    void update();
    void render();

    // TODO those methods potentially should not use dynamic dispatching
    // consider adding some CRTP pattern to call methods from derived class
    virtual void onInit() = 0;
    virtual void onClose() = 0;
    virtual void onEvent(const sf::Event& event) = 0;
    virtual void onUpdate(float timeStep) = 0;
    virtual void onRender() = 0;

    bool isRunning_{true};

    sf::RenderWindow window_;
    sf::Clock clock_;

    engine::ParticleSystem particleSystem_;
    engine::TimerService timerService_;
    Context context_;

    engine::input::KeyboardHandler keyboardHandler_;
    engine::input::MouseHandler mouseHandler_;

    float constantTimeStep_{1.0f/30.f};
    Clock::duration realTimeStep_;
    engine::Scene scene_;
    engine::CollisionSolver collisionSolver_;
    engine::Logger logger_;
};

}  // namespace engine
