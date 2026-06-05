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
, cameraInitialSize_{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}
, cameraWorldSize_{Config::WINDOW_WIDTH * 4.f, Config::WINDOW_HEIGHT * 4.f}
, cameraInitialPosition_{0.f + Config::WINDOW_WIDTH / 2.f, 0.f + Config::WINDOW_HEIGHT / 2.f}
, camera_{cameraInitialPosition_, cameraInitialSize_, cameraWorldSize_}
, cameraController_{&camera_}
, cameraView_{sf::FloatRect(0.f, 0.f, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT)}
, waypointMouseController_{waypoints_, window_, cameraView_}
, fpsAverage_{NUMBER_OF_MEASUREMENTS}
, drawProfiler_{"game::Application::DRAW", "ms"}
, physicsProfiler_{"game::Application::PHYSICS", "ms"}
, guiProfiler_{"game::Application::GUI", "ms"}
, luaProfiler_{"game::lua", "ms"}
, tilemapProfiler_{"game::Tilemap::draw", "ms"}
, renderGameObjectsProfiler_{"game::renderGameObjects", "ms"}
, tracksProfiler_{"game::TracksRenderer::draw", "ms"}
, waypointsProfiler_{"game::drawWaypoints", "ms"}
, particlesProfiler_{"game::ParticleSystem::draw", "ms"}
, displayProfiler_{"engine::window.display", "ms"}
{}

void Application::onInit()
{
    context_.setCamera(&camera_);
    graphics::TextureLibrary::init();

    tilemap_ = std::make_unique<graphics::Tilemap>();
    mouseHandler_.subscribe(&waypointMouseController_);

    window_.setFramerateLimit(60);
    window_.setVerticalSyncEnabled(false);

    cameraView_.setCenter(cameraInitialPosition_);
    configureGUI();

    keyboardHandler_.subscribe(std::vector<sf::Keyboard::Key>
    {
        sf::Keyboard::W,
        sf::Keyboard::S,
        sf::Keyboard::A,
        sf::Keyboard::D
    }, &cameraController_);

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
                case sf::Keyboard::F8       :   {constantTimeStep_ = 1.0f/300.f;} break;
                case sf::Keyboard::F9       :   {constantTimeStep_ = 1.0f/150.f;} break;
                case sf::Keyboard::F10      :   {constantTimeStep_ = 1.0f/30.f;} break;
                case sf::Keyboard::F11      :   {rigidBodyDebug_ = !rigidBodyDebug_;} break;
                case sf::Keyboard::F12      :   {tankDebugMode_=!tankDebugMode_; entity::Tank::setDebug(tankDebugMode_);} break;
                case sf::Keyboard::T        :   tracksRenderer_.clear(); break;
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

void Application::onUpdate(float timeStep)
{
    gui::Application::onUpdate(timeStep);
    clock_.restart();
    PROFILE_SCOPE(luaProfiler_,
        scriptsScheduler_.update(timeStep);
    );

    camera_.update(timeStep);
    cameraView_.setCenter(camera_.getPosition());
    cameraView_.setSize(camera_.getSize());
    
    PROFILE_SCOPE(physicsProfiler_,
        for (auto& object : scene_.objects())
        {
            object->update(scene_, constantTimeStep_);
        }
        collisionSolver_.evaluateCollisions();
    );

}

void Application::onRender()
{
    // End measurement of previous frame respecting vsync and frame limiting of SFML lib
    // otherwise measurements are measuring potential FPS (not actual/frame limited)
    fpsCounter_.endMeasurement();
    //TODO move this FPS limiting and counting to base class
    // otherwise values are wrong
    fpsCounter_.startMeasurement();

    window_.setView(cameraView_);
    
    PROFILE_SCOPE(tilemapProfiler_,
        tilemap_->draw(window_);
    );
    
    PROFILE_SCOPE(waypointsProfiler_,
        graphics::drawtools::drawWaypoints(window_, waypoints_);
    );
    
    PROFILE_SCOPE(tracksProfiler_,
        tracksRenderer_.draw(window_);
    );
    
    PROFILE_SCOPE(renderGameObjectsProfiler_,
        renderGameObjects();
    );
    
    PROFILE_SCOPE(particlesProfiler_,
        particleSystem_.draw(window_);
    );

    if (rigidBodyDebug_)
    {
        engine::RigidBodyDebugRenderer::debug(scene_, window_);
    }

    window_.setView(window_.getDefaultView());

    PROFILE_SCOPE(guiProfiler_,
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
        const auto rb = object->getRigidBody();
        if (not cameraFrustum.contains(rb.x_, rb.y_))
        {
            continue;
        }

        auto distanceFromCameraCenter = engine::math::distance(cameraPosition.x, cameraPosition.y, rb.x_, rb.y_);

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
        e.object->draw(window_, constantTimeStep_);
    }

    objectsToDraw.clear();
}

void Application::configureGUI()
{
    auto quitButton = gui::TextButton::create("Quit");
    quitButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 100.f));
    quitButton->setSize(sf::Vector2f(150.f, 30.f));
    quitButton->onClick([this](){logger_.info("Quitting..."); Application::close();});
    gui().mainWindow().addChild(std::move(quitButton));

    auto button = gui::TextButton::create("Help");
    button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 150.f));
    button->setSize(sf::Vector2f(150.f, 30.f));
    button->onClick([this](){
        auto helpWindow = std::make_unique<game::HelpWindow>(sf::Vector2f(300.f, 300.f));
        helpWindow->setTitle("Help");
        gui().openWindow(std::move(helpWindow));
    });
    gui().mainWindow().addChild(std::move(button));

    auto reloadLuaButton = gui::TextButton::create("Reload Lua scripts");
    reloadLuaButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 200.f));
    reloadLuaButton->setSize(sf::Vector2f(150.f, 30.f));
    reloadLuaButton->onClick([this](){
        for (auto& controller : luaControllers_)
        {
            controller->reload();
            scriptsScheduler_.add(controller->getScript());
        }
    });
    gui().mainWindow().addChild(std::move(reloadLuaButton));

    auto menuButton = gui::TextButton::create("Menu");
    menuButton->setPosition(sf::Vector2f(20.f, 250.f));
    menuButton->setSize(sf::Vector2f(150.f, 30.f));
    auto* menuButtonPtr = menuButton.get();
    menuButton->onClick([this, menuButtonPtr]() {
        const auto menuPosition = menuButtonPtr->getGlobalPosition() + sf::Vector2f{0.f, 32.f};
        auto menu = gui::ContextMenu::create(
            {
                {"Reset camera", [this]() { camera_.setPosition(cameraInitialPosition_.x, cameraInitialPosition_.y); camera_.resetZoom();}},
                {"Options >", {},
                    {
                        {"Toggle debug", [this]() { tankDebugMode_ = !tankDebugMode_; entity::Tank::setDebug(tankDebugMode_); }},
                        {"Time  >", {}, {
                            {"1x (normal)", [this](){constantTimeStep_ = 1.0f/30.f;}},
                            {"5x slower",   [this](){constantTimeStep_ = 1.0f/150.f;}},
                            {"10x slower",  [this](){constantTimeStep_ = 1.0f/300.f;}},
                        }},
                        {"Close", {}},
                    }
                }
            }
        );

        gui().openContextMenu(std::move(menu), menuPosition);
    });
    
    gui().mainWindow().addChild(std::move(menuButton));

    gui().mainWindow().setContextMenuHandler([this](const sf::Vector2f& pos) {
        auto menu = gui::ContextMenu::create(
            {
                {"Reload Lua", [this]() { for (auto& controller : luaControllers_) { controller->reload(); scriptsScheduler_.add(controller->getScript()); } }},
                {"Toggle debug", [this]() { tankDebugMode_ = !tankDebugMode_; entity::Tank::setDebug(tankDebugMode_); }},
                {"Reset camera", [this]() { camera_.setPosition(cameraInitialPosition_.x, cameraInitialPosition_.y); camera_.resetZoom();}},
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
        const auto xSpawnPosition = i * 100 + 100;
        const auto ySpawnPosition = xSpawnPosition;
        const auto spawnRotation = i * 36;
        auto tank = entity::TankFactory::create(
            static_cast<entity::TankFactory::TankType>(i),
            xSpawnPosition, ySpawnPosition, spawnRotation, &tracksRenderer_);

        auto navigator = std::make_unique<game::entity::TankController>("scripts/navigator.lua", tank.get(), waypoints_);
        scene_.spawnObject(std::move(tank));
        scriptsScheduler_.add(navigator->getScript());
        luaControllers_.push_back(std::move(navigator));
    }

    auto turtleTank = entity::TankFactory::create(
        entity::TankFactory::TankType::Green, Config::WINDOW_WIDTH / 2.f + 100.f, Config::WINDOW_HEIGHT/2.f, 0.f, &tracksRenderer_);

    turtleTank->led().setColor(sf::Color::Red);
    auto turtleTankController = std::make_unique<game::entity::TankController>("scripts/turtle.lua", turtleTank.get(), waypoints_);
    scriptsScheduler_.add(turtleTankController->getScript());
    scene_.spawnObject(std::move(turtleTank));
    luaControllers_.push_back(std::move(turtleTankController));

    auto ledTank = entity::TankFactory::create(
        entity::TankFactory::TankType::Red, Config::WINDOW_WIDTH / 2.f + 200.f, Config::WINDOW_HEIGHT/2.f, 0.f, &tracksRenderer_);

    ledTank->led().setColor(sf::Color::Red);
    auto ledTankController = std::make_unique<game::entity::TankController>("scripts/blink.lua", ledTank.get(), waypoints_);
    scriptsScheduler_.add(ledTankController->getScript());
    scene_.spawnObject(std::move(ledTank));
    luaControllers_.push_back(std::move(ledTankController));
}

void Application::spawnSomeBarrelsAndCratesAndTress()
{
    for (size_t i = 0; i < BARRELS_COUNT; ++i)
    {
        const auto xSpawnPosition = i * 30 + 500;
        const auto ySpawnPosition = xSpawnPosition - 400;

        scene_.spawnObject(entity::BarrelFactory::create(
            static_cast<entity::BarrelFactory::BarrelType>(i % 4),
            xSpawnPosition, ySpawnPosition));
    }

    for (size_t i = BARRELS_COUNT; i < CRATES_COUNT + BARRELS_COUNT; ++i)
    {
        const auto xSpawnPosition = i * 30 + 500;
        const auto ySpawnPosition = xSpawnPosition - 400;

        scene_.spawnObject(entity::CrateFactory::create(
            static_cast<entity::CrateFactory::CrateType>(i % 2),
            xSpawnPosition, ySpawnPosition));
    }

    constexpr size_t NUMBER_OF_TREES_OF_EACH_TYPE = 8;

    using Type = game::entity::TreeFactory::TreeType;
    const auto treeTypes = std::vector<Type>
    {
        Type::Brown_Large,
        Type::Brown_Small,
        Type::Green_Large,
        Type::Green_Small
    };

    for (const auto& treeType : treeTypes)
    {
        for (size_t i = 0; i < NUMBER_OF_TREES_OF_EACH_TYPE; ++i)
        {
            const auto xPosition = rand() % static_cast<int>(cameraWorldSize_.x);
            const auto yPosition = rand() % static_cast<int>(cameraWorldSize_.y);
            scene_.spawnObject(game::entity::TreeFactory::create(
                treeType, xPosition, yPosition));
        }
    }
}

void Application::generateProfiling()
{
    //TODO: move fps counting to engine application part
    engine::ProfileResult result {
        .name = "FPS",
        .unit = "",
        .lastFrame = static_cast<float>(fpsCounter_.getFps()),
        .average = (fpsAverage_.calculate(fpsCounter_.getFps()))
    };
    profiling_.store(result);
}

}  // namespace game
