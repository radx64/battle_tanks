#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include <fmt/format.h>

#include "engine/Logger.hpp"
#include "engine/math/Math.hpp"
#include "engine/RigidBodyDebugRenderer.hpp"

#include "game/Application.hpp"
#include "game/entity/barrel/Barrel.hpp"
#include "game/entity/barrel/BarrelFactory.hpp"
#include "game/entity/crate/Crate.hpp"
#include "game/entity/crate/CrateFactory.hpp"
#include "game/entity/tank/TankFactory.hpp"
#include "game/entity/tree/Tree.hpp"
#include "game/entity/tree/TreeFactory.hpp"
#include "game/HelpWindow.hpp"

#include "lua/ScriptContext.hpp"

#include "graphics/DrawTools.hpp"
#include "graphics/TextureLibrary.hpp"

#include "gui/Button.hpp"
#include "gui/Event.hpp"
#include "gui/Window.hpp"
#include "gui/ContextMenu.hpp"

#include "Config.hpp"

namespace game
{

constexpr size_t TANKS_COUNT = 5;
constexpr size_t BARRELS_COUNT = 10;
constexpr size_t CRATES_COUNT = 10;

constexpr int NUMBER_OF_MEASUREMENTS = 10;

Application::Application()
: gui::Application("Battle tanks", "Battle tanks", {Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT})
, camera_initial_size_{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}
, camera_world_size_{Config::WINDOW_WIDTH * 4.f, Config::WINDOW_HEIGHT * 4.f}
, camera_initial_position_{0.f + Config::WINDOW_WIDTH / 2.f, 0.f + Config::WINDOW_HEIGHT / 2.f}
, camera_{camera_initial_position_, camera_initial_size_, camera_world_size_}
, camera_controller_{&camera_}
, camera_view_{sf::FloatRect(0.f, 0.f, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT)}
, waypoint_mouse_controller_{waypoints_, window_, camera_view_}
, fps_average_{NUMBER_OF_MEASUREMENTS}
, draw_profiler_{"game::Application::DRAW", "ms"}
, physics_profiler_{"game::Application::PHYSICS", "ms"}
, gui_profiler_{"game::Application::GUI", "ms"}
, lua_profiler_{"game::lua", "ms"}
, tilemap_profiler_{"game::Tilemap::draw", "ms"}
, render_game_objects_profiler_{"game::renderGameObjects", "ms"}
, tracks_profiler_{"game::TracksRenderer::draw", "ms"}
, waypoints_profiler_{"game::drawWaypoints", "ms"}
, particles_profiler_{"game::ParticleSystem::draw", "ms"}
, display_profiler_{"engine::window.display", "ms"}
{}

void Application::onInit()
{
    context_.setCamera(&camera_);
    graphics::TextureLibrary::init();

    tilemap_ = std::make_unique<graphics::Tilemap>();
    mouseHandler_.subscribe(&waypoint_mouse_controller_);

    window_.setFramerateLimit(60);
    window_.setVerticalSyncEnabled(false);

    camera_view_.setCenter(camera_initial_position_);
    configureGUI();

    keyboardHandler_.subscribe(std::vector<sf::Keyboard::Key>
    {
        sf::Keyboard::W,
        sf::Keyboard::S,
        sf::Keyboard::A,
        sf::Keyboard::D
    }, &camera_controller_);

    spawnSomeTanks();
    spawnSomeBarrelsAndCratesAndTress();
}

void Application::onClose()
{
    logger_.info("Goodbye!");
}

void Application::onEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::KeyReleased :
        {
            switch (event.key.code)
            {
                case sf::Keyboard::PageUp   :   camera_.zoomIn(); break;
                case sf::Keyboard::PageDown :   camera_.zoomOut(); break;
                case sf::Keyboard::C        :   waypoints_.clear(); break;
                case sf::Keyboard::F8       :   {constanttime_step_ = 1.0f/300.f;} break;
                case sf::Keyboard::F9       :   {constanttime_step_ = 1.0f/150.f;} break;
                case sf::Keyboard::F10      :   {constanttime_step_ = 1.0f/30.f;} break;
                case sf::Keyboard::F11      :   {rigid_body_debug_ = !rigid_body_debug_;} break;
                case sf::Keyboard::F12      :   {tank_debug_mode_=!tank_debug_mode_; entity::Tank::setDebug(tank_debug_mode_);} break;
                case sf::Keyboard::T        :   tracks_renderer_.clear(); break;
                case sf::Keyboard::F        :   if(!waypoints_.empty()) waypoints_.pop_back(); break;
                case sf::Keyboard::Q        :   Application::close();
                default                     :   {}
            }
            break;
        }

        case sf::Event::MouseWheelMoved :
        {
            if (event.mouseWheel.delta > 0) camera_.zoomIn(event.mouseWheel.x, event.mouseWheel.y);
            if (event.mouseWheel.delta < 0) camera_.zoomOut();
        }
        default : {}
    }
}

void Application::onUpdate(float time_step)
{
    gui::Application::onUpdate(time_step);
    clock_.restart();
    PROFILE_SCOPE(lua_profiler_,
        scripts_scheduler_.update(time_step);
    );

    camera_.update(time_step);
    camera_view_.setCenter(camera_.getPosition());
    camera_view_.setSize(camera_.getSize());
    
    PROFILE_SCOPE(physics_profiler_,
        for (auto& object : scene_.objects())
        {
            object->update(scene_, constanttime_step_);
        }
        collisionSolver_.evaluateCollisions();
    );

}

void Application::onRender()
{
    // End measurement of previous frame respecting vsync and frame limiting of SFML lib
    // otherwise measurements are measuring potential FPS (not actual/frame limited)
    fps_counter_.endMeasurement();
    //TODO move this FPS limiting and counting to base class
    // otherwise values are wrong
    fps_counter_.startMeasurement();

    window_.setView(camera_view_);
    
    PROFILE_SCOPE(tilemap_profiler_,
        tilemap_->draw(window_);
    );
    
    PROFILE_SCOPE(waypoints_profiler_,
        graphics::drawtools::drawWaypoints(window_, waypoints_);
    );
    
    PROFILE_SCOPE(tracks_profiler_,
        tracks_renderer_.draw(window_);
    );
    
    PROFILE_SCOPE(render_game_objects_profiler_,
        renderGameObjects();
    );
    
    PROFILE_SCOPE(particles_profiler_,
        particle_system_.draw(window_);
    );

    if (rigid_body_debug_)
    {
        engine::RigidBodyDebugRenderer::debug(scene_, window_);
    }

    window_.setView(window_.getDefaultView());

    PROFILE_SCOPE(gui_profiler_,
        gui::Application::onRender();
    );

    clock_.restart();

    generateProfiling();
}

void Application::renderGameObjects()
{
    // Crude concept for correct depth rendering
    // Consider static vector size and it's single instance
    struct GameObjectWithDistanceToCamera
    {
        engine::GameObject* object;
        float distance;
    };

    static std::vector<GameObjectWithDistanceToCamera> objectsToDraw;
    objectsToDraw.reserve(scene_.objects().size());

    const auto& cameraPosition = camera_.getPosition();
    sf::Rect<float> cameraFrustum {cameraPosition - camera_.getSize()/2.f , camera_.getSize() };

    for (auto& object : scene_.objects())
    {
        const auto rb = object->transform();
        if (not cameraFrustum.contains(rb.position().x, rb.position().y))
        {
            continue;
        }

        auto distanceFromCameraCenter = engine::math::distance(cameraPosition.x, cameraPosition.y, rb.position().x, rb.position().y);

        objectsToDraw.emplace_back(GameObjectWithDistanceToCamera{object.get(), distanceFromCameraCenter});
    }

    std::sort(objectsToDraw.begin(), objectsToDraw.end(),
        [](const auto& left, const auto& right)
        {
            return left.distance > right.distance;
        }
    );

    for (auto& e : objectsToDraw)
    {
        e.object->draw(window_, constanttime_step_);
    }

    objectsToDraw.clear();
}

void Application::configureGUI()
{
    auto quit_button = gui::TextButton::create("Quit");
    quit_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 100.f));
    quit_button->setSize(sf::Vector2f(150.f, 30.f));
    quit_button->onClick([this](){logger_.info("Quitting..."); Application::close();});
    gui().mainWindow().addChild(std::move(quit_button));

    auto button = gui::TextButton::create("Help");
    button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 150.f));
    button->setSize(sf::Vector2f(150.f, 30.f));
    button->onClick([this](){
        auto help_window = std::make_unique<game::HelpWindow>(sf::Vector2f(300.f, 300.f));
        help_window->setTitle("Help");
        gui().openWindow(std::move(help_window));
    });
    gui().mainWindow().addChild(std::move(button));

    auto reload_lua_button = gui::TextButton::create("Reload Lua scripts");
    reload_lua_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 200.f));
    reload_lua_button->setSize(sf::Vector2f(150.f, 30.f));
    reload_lua_button->onClick([this](){
        for (auto& controller : lua_controllers_)
        {
            controller->reload();
            scripts_scheduler_.add(controller->getScript());
        }
    });
    gui().mainWindow().addChild(std::move(reload_lua_button));

    auto menu_button = gui::TextButton::create("Menu");
    menu_button->setPosition(sf::Vector2f(20.f, 250.f));
    menu_button->setSize(sf::Vector2f(150.f, 30.f));
    auto* menu_button_ptr = menu_button.get();
    menu_button->onClick([this, menu_button_ptr]() {
        const auto menu_position = menu_button_ptr->getGlobalPosition() + sf::Vector2f{0.f, 32.f};
        auto menu = gui::ContextMenu::create(
            {
                {"Reset camera", [this]() { camera_.setPosition(camera_initial_position_.x, camera_initial_position_.y); camera_.resetZoom();}},
                {"Options >", {},
                    {
                        {"Toggle debug", [this]() { tank_debug_mode_ = !tank_debug_mode_; entity::Tank::setDebug(tank_debug_mode_); }},
                        {"Time  >", {}, {
                            {"1x (normal)", [this](){constanttime_step_ = 1.0f/30.f;}},
                            {"5x slower",   [this](){constanttime_step_ = 1.0f/150.f;}},
                            {"10x slower",  [this](){constanttime_step_ = 1.0f/300.f;}},
                        }},
                        {"Close", {}},
                    }
                }
            }
        );

        gui().openContextMenu(std::move(menu), menu_position);
    });
    
    gui().mainWindow().addChild(std::move(menu_button));

    gui().mainWindow().setContextMenuHandler([this](const sf::Vector2f& pos) {
        auto menu = gui::ContextMenu::create(
            {
                {"Reload Lua", [this]() { for (auto& controller : lua_controllers_) { controller->reload(); scripts_scheduler_.add(controller->getScript()); } }},
                {"Toggle debug", [this]() { tank_debug_mode_ = !tank_debug_mode_; entity::Tank::setDebug(tank_debug_mode_); }},
                {"Reset camera", [this]() { camera_.setPosition(camera_initial_position_.x, camera_initial_position_.y); camera_.resetZoom();}},
            }
        );
        menu->open(pos);
        gui().openContextMenu(std::move(menu), pos);
    });
}

void Application::spawnSomeTanks()
{
    for (size_t i = 0; i < TANKS_COUNT; ++i)
    {
        const auto x_spawn_position = i * 100 + 100;
        const auto y_spawn_position = x_spawn_position;
        const auto spawnRotation = i * 36;
        auto tank = entity::TankFactory::create(
            static_cast<entity::TankFactory::TankType>(i),
            x_spawn_position, y_spawn_position, spawnRotation, &tracks_renderer_);

        auto navigator = std::make_unique<game::entity::TankController>("scripts/navigator.lua", tank.get(), waypoints_);
        scene_.spawnObject(std::move(tank));
        scripts_scheduler_.add(navigator->getScript());
        lua_controllers_.push_back(std::move(navigator));
    }

    auto turtle_tank = entity::TankFactory::create(
        entity::TankFactory::TankType::Green, Config::WINDOW_WIDTH / 2.f + 100.f, Config::WINDOW_HEIGHT/2.f, 0.f, &tracks_renderer_);

    turtle_tank->led().setColor(sf::Color::Red);
    auto turtle_tank_controller = std::make_unique<game::entity::TankController>("scripts/turtle.lua", turtle_tank.get(), waypoints_);
    scripts_scheduler_.add(turtle_tank_controller->getScript());
    scene_.spawnObject(std::move(turtle_tank));
    lua_controllers_.push_back(std::move(turtle_tank_controller));

    auto led_tank = entity::TankFactory::create(
        entity::TankFactory::TankType::Red, Config::WINDOW_WIDTH / 2.f + 200.f, Config::WINDOW_HEIGHT/2.f, 0.f, &tracks_renderer_);

    led_tank->led().setColor(sf::Color::Red);
    auto led_tank_controller = std::make_unique<game::entity::TankController>("scripts/blink.lua", led_tank.get(), waypoints_);
    scripts_scheduler_.add(led_tank_controller->getScript());
    scene_.spawnObject(std::move(led_tank));
    lua_controllers_.push_back(std::move(led_tank_controller));
}

void Application::spawnSomeBarrelsAndCratesAndTress()
{
    for (size_t i = 0; i < BARRELS_COUNT; ++i)
    {
        const auto x_spawn_position = i * 30 + 500;
        const auto y_spawn_position = x_spawn_position - 400;

        scene_.spawnObject(entity::BarrelFactory::create(
            static_cast<entity::BarrelFactory::BarrelType>(i % 4),
            x_spawn_position, y_spawn_position));
    }

    for (size_t i = BARRELS_COUNT; i < CRATES_COUNT + BARRELS_COUNT; ++i)
    {
        const auto x_spawn_position = i * 30 + 500;
        const auto y_spawn_position = x_spawn_position - 400;

        scene_.spawnObject(entity::CrateFactory::create(
            static_cast<entity::CrateFactory::CrateType>(i % 2),
            x_spawn_position, y_spawn_position));
    }

    constexpr size_t NUMBER_OF_TREES_OF_EACH_TYPE = 8;

    using Type = game::entity::TreeFactory::TreeType;
    const auto tree_types = std::vector<Type>
    {
        Type::Brown_Large,
        Type::Brown_Small,
        Type::Green_Large,
        Type::Green_Small
    };

    for (const auto& tree_type : tree_types)
    {
        for (size_t i = 0; i < NUMBER_OF_TREES_OF_EACH_TYPE; ++i)
        {
            const auto x_position = rand() % static_cast<int>(camera_world_size_.x);
            const auto y_position = rand() % static_cast<int>(camera_world_size_.y);
            scene_.spawnObject(game::entity::TreeFactory::create(
                tree_type, x_position, y_position));
        }
    }
}

void Application::generateProfiling()
{
    //TODO: move fps counting to engine application part
    engine::ProfileResult result {
        .name = "FPS",
        .unit = "",
        .lastFrame = static_cast<float>(fps_counter_.getFps()),
        .average = (fps_average_.calculate(fps_counter_.getFps()))
    };
    profiling_.store(result);
}

}  // namespace game
