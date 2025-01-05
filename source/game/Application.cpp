#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

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

#include "graphics/DrawTools.hpp"
#include "graphics/TextureLibrary.hpp"

#include "gui/Button.hpp"
#include "gui/Event.hpp"
#include "gui/Label.hpp"
#include "gui/Layout.hpp"
#include "gui/Window.hpp"

#include "Config.hpp"

namespace game
{


constexpr size_t TANKS_COUNT = 5;
constexpr size_t BARRELS_COUNT = 10;
constexpr size_t CRATES_COUNT = 10;

constexpr int NUMBER_OF_MEASUREMENTS = 10;

Application::Application()
: engine::Application("Battle tanks")
, cameraInitialPosition_{Config::WINDOW_WIDTH/2.f, Config::WINDOW_HEIGHT/2.f}
, cameraInitialSize_{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}
, camera_{cameraInitialPosition_, cameraInitialSize_}
, cameraController_{&camera_}
, cameraView_{sf::FloatRect(0.f, 0.f, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT)}
, windowManager_{sf::Vector2f{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}}
, waypointMouseController_{&windowManager_, waypoints_, window_, cameraView_}
, guiMouseController_{&windowManager_, window_, window_.getDefaultView()}
, drawAverage_{NUMBER_OF_MEASUREMENTS}
, physicsAverage_{NUMBER_OF_MEASUREMENTS}
, navAverage_{NUMBER_OF_MEASUREMENTS}
, fpsAverage_{NUMBER_OF_MEASUREMENTS}
, guiAverage_{NUMBER_OF_MEASUREMENTS}
{}

void Application::onInit()
{
    context_.setParticleSystem(&particleSystem_);
    context_.setScene(&scene_);
    context_.setCamera(&camera_);
    gui::FontLibrary::initialize();
    graphics::TextureLibrary::initialize();
    tilemap_ = std::make_unique<graphics::Tilemap>();

    mouseHandler_.subscribe(&guiMouseController_);
    mouseHandler_.subscribe(&waypointMouseController_);

    window_.setFramerateLimit(60);
    window_.setVerticalSyncEnabled(true);

    cameraView_.setCenter(Config::WINDOW_WIDTH/2.0, Config::WINDOW_HEIGHT/2.0);
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
    std::cout << "Goodbye!" << std::endl;
    gui::FontLibrary::destroy();
    graphics::TextureLibrary::destroy();
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
    clock_.restart();
    for(auto& navigator : navigators_)
    {
        navigator->navigate();
    }
    navTime_ = clock_.getElapsedTime();

    clock_.restart();
    camera_.update(timeStep);
    cameraView_.setCenter(camera_.getPosition());
    cameraView_.setSize(camera_.getSize());
    for (auto& object : scene_.objects())
    {
        object->update(scene_, constantTimeStep_);
    }
    collisionSolver_.evaluateCollisions();
    physicsTime_ = clock_.getElapsedTime();
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
    tilemap_->draw(window_);
    graphics::drawtools::drawWaypoints(window_, waypoints_);
    tracksRenderer_.draw(window_);
    renderGameObjects();
    particleSystem_.draw(window_);
    drawTime_ = clock_.getElapsedTime().asMilliseconds();

    if (rigidBodyDebug_)
    {
        engine::RigidBodyDebugRenderer::debug(scene_, window_);
    }

    window_.setView(window_.getDefaultView());
    windowManager_.render(window_);

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
    auto quitButton = std::make_unique<gui::Button>("Quit");
    quitButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 100.f));
    quitButton->setSize(sf::Vector2f(150.f, 30.f));
    quitButton->onClick([this](){engine::Logger::info("Quitting..."); Application::close();});
    windowManager_.mainWindow().addChild(std::move(quitButton));

    auto measurementsText = std::make_unique<gui::Label>("");
    measurementsText->setPosition(sf::Vector2f(20.f, 20.f));
    measurementsTextHandle_ = measurementsText.get();

    windowManager_.mainWindow().addChild(std::move(measurementsText));

    auto measurementsAverageText = std::make_unique<gui::Label>("");
    measurementsAverageText->setPosition(sf::Vector2f(200.f, 20.f));
    measurementsAverageTextHandle_ = measurementsAverageText.get();

    windowManager_.mainWindow().addChild(std::move(measurementsAverageText));

    auto button = std::make_unique<gui::Button>("Help");
    button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 150.f));
    button->setSize(sf::Vector2f(150.f, 30.f));
    button->onClick([this](){
        auto helpWindow = std::make_unique<game::HelpWindow>(sf::Vector2f(Config::WINDOW_WIDTH/2, 600.0f));
        helpWindow->setTitle("Help");
        helpWindow->setPosition({80.f,80.f});
        windowManager_.addWindow(std::move(helpWindow));
    });
    windowManager_.mainWindow().addChild(std::move(button));

    auto spawnWindowButton = std::make_unique<gui::Button>("Spawn new window");
    spawnWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 250.f));
    spawnWindowButton->setSize(sf::Vector2f(150.f, 30.f));
    spawnWindowButton->onClick([this](){
        float randomX = rand() % 200;
        float randomY = rand() % 200;
        auto window = std::make_unique<gui::Window>();

        auto horizontalLayout = std::make_unique<gui::HorizontalLayout>();
        auto helloButton = std::make_unique<gui::Button>("HELLO");
        auto worldButton = std::make_unique<gui::Button>("WORLD");

        helloButton->onClick([](){std::cout << "Hello?" << std::endl;});
        worldButton->onClick([](){std::cout << "Is it me you looking for?" << std::endl;});

        horizontalLayout->addChild(std::move(helloButton));
        horizontalLayout->addChild(std::move(worldButton));

        auto horizontalLayout2   = std::make_unique<gui::HorizontalLayout>();
        auto testing = std::make_unique<gui::Button>("TESTING");
        auto things = std::make_unique<gui::Button>("THINGS");

        horizontalLayout2->addChild(std::move(testing));
        horizontalLayout2->addChild(std::move(things));

        auto verticalLayout = std::make_unique<gui::VerticalLayout>();

        verticalLayout->addChild(std::move(horizontalLayout));
        verticalLayout->addChild(std::move(horizontalLayout2));

        window->addChild(std::move(verticalLayout));

        window->setSize(sf::Vector2f(500.0f, 400.0f));
        window->setPosition(sf::Vector2f((Config::WINDOW_WIDTH+randomX)/2, 400.0f+randomY));
        window->setTitle("Oh my gosh");

        windowManager_.addWindow(std::move(window));
    });
    windowManager_.mainWindow().addChild(std::move(spawnWindowButton));
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

        auto navigator = std::make_unique<Navigator>(*tank, waypoints_);
        scene_.spawnObject(std::move(tank));
        navigators_.push_back(std::move(navigator));
    }
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
            const auto xPosition = rand() % Config::WINDOW_WIDTH;
            const auto yPosition = rand() % Config::WINDOW_HEIGHT;
            scene_.spawnObject(game::entity::TreeFactory::create(
                treeType, xPosition, yPosition));
        }
    }
}

void Application::generateProfiling()
{
    // TODO: Create some proper profiler class not that
    measurementsTextHandle_->setText("DRAW: " + std::to_string(drawTime_)
            + "ms\nPHYSICS: " + std::to_string(physicsTime_.asMicroseconds())
            + "us\nNAV: " + std::to_string(navTime_.asMicroseconds())
            + "us\nGUI: " + std::to_string(guiTime_.asMilliseconds())
            + "ms\nFPS: "+ std::to_string(fpsCounter_.getFps())
            + "\nObjects count: " + std::to_string(scene_.objects().size()));

    measurementsAverageTextHandle_->setText("AVG: " + std::to_string(drawAverage_.calculate(drawTime_))
        + "ms\nAVG: " + std::to_string(physicsAverage_.calculate(physicsTime_.asMicroseconds()))
        + "us\nAVG: " + std::to_string(navAverage_.calculate(navTime_.asMicroseconds()))
        + "us\nAVG: " + std::to_string(guiAverage_.calculate(guiTime_.asMilliseconds()))
        + "ms\nAVG: " + std::to_string(fpsAverage_.calculate(fpsCounter_.getFps())));
}

}  // namespace game
