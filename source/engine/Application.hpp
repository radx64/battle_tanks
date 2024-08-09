#pragma once

#include <SFML/Graphics.hpp>

#include "engine/CollisionSolver.hpp"
#include "engine/input/KeyboardHandler.hpp"
#include "engine/input/MouseHandler.hpp"
#include "engine/ParticleSystem.hpp"
#include "engine/Scene.hpp"

namespace engine
{

class Application
{
public:
    Application(const std::string_view& windowName);
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

    bool is_running_{true};

    sf::RenderWindow window_;
    sf::Clock clock_;

    engine::ParticleSystem particle_system_;
    engine::input::KeyboardHandler keyboard_handler_;
    engine::input::MouseHandler mouse_handler_;

    float timeStep_{1.0f/30.f};
    engine::Scene scene_;
    engine::CollisionSolver collision_solver_;

};

}  // namespace engine

