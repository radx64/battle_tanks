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

#include "game/Application.hpp"
#include "game/entity/barrel/Barrel.hpp"
#include "game/entity/barrel/BarrelFactory.hpp"
#include "game/entity/crate/Crate.hpp"
#include "game/entity/crate/CrateFactory.hpp"
#include "game/entity/tank/TankFactory.hpp"
#include "game/entity/tree/Tree.hpp"
#include "game/entity/tree/TreeFactory.hpp"
#include "game/HelpWindow.hpp"
#include "engine/RigidBodyDebugRenderer.hpp"
#include "graphics/DrawTools.hpp"
#include "graphics/TextureLibrary.hpp"
#include "gui/Button.hpp"
#include "gui/Event.hpp"
#include "gui/Label.hpp"
#include "gui/Layout.hpp"
#include "gui/Window.hpp"
#include "engine/math/Math.hpp"

#include "Config.hpp"

namespace game 
{


constexpr size_t TANKS_COUNT = 5;
constexpr size_t BARRELS_COUNT = 10;
constexpr size_t CRATES_COUNT = 10;

constexpr int NUMBER_OF_MEASUREMENTS = 10;

Application::Application()
: engine::Application("Battle tanks")
, camera_initial_position_{Config::WINDOW_WIDTH/2.f, Config::WINDOW_HEIGHT/2.f}
, camera_initial_size_{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}
, camera_{camera_initial_position_, camera_initial_size_}
, camera_controller_{&camera_}
, camera_view_{sf::FloatRect(0.f, 0.f, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT)}
, draw_average_{NUMBER_OF_MEASUREMENTS}
, physics_average_{NUMBER_OF_MEASUREMENTS}
, nav_average_{NUMBER_OF_MEASUREMENTS}
, fps_average_{NUMBER_OF_MEASUREMENTS}
, gui_average_{NUMBER_OF_MEASUREMENTS}
{}

void Application::onInit()
{
    context_.setParticleSystem(&particle_system_);
    context_.setScene(&scene_);
    context_.setCamera(&camera_);
    // FIXME static init of fonts and textures can lead to crashes on exit as
    // statics can be destroyed after sfml context so this is a bit temporary
    // solution until better staticless asset manager
    gui::FontLibrary::initialize(); 
    graphics::TextureLibrary::initialize();
    tilemap_ = std::make_unique<graphics::Tilemap>();
    window_manager_ = std::make_unique<gui::WindowManager>(sf::Vector2f{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT});

    // window_.setFramerateLimit(120);
    // fpsLimiter_.setFrameLimit(1000);
    // window_.setVerticalSyncEnabled(false);
    
    camera_view_.setCenter(Config::WINDOW_WIDTH/2.0, Config::WINDOW_HEIGHT/2.0);
    configureGUI();

    keyboard_handler_.subscribe(std::vector<sf::Keyboard::Key>
    {
        sf::Keyboard::W,
        sf::Keyboard::S,
        sf::Keyboard::A,
        sf::Keyboard::D
    }, &camera_controller_);

    spawnSomeTanks();
    spawnSomeBarrelsAndCratesAndTress();
}


void Application::onEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::Closed : { Application::close(); break; }
        case sf::Event::KeyPressed :
        {
            keyboard_handler_.handleKeyPressed(event.key);
            break;
        } 
        case sf::Event::KeyReleased : 
        {
            keyboard_handler_.handleKeyReleased(event.key);
            switch (event.key.code)
            {
                case sf::Keyboard::PageUp   :   camera_.zoomIn(); break;
                case sf::Keyboard::PageDown :   camera_.zoomOut(); break;
                case sf::Keyboard::C        :   waypoints_.clear(); break;
                case sf::Keyboard::F8       :   {timeStep_ = 1.0f/300.f;} break;
                case sf::Keyboard::F9       :   {timeStep_ = 1.0f/150.f;} break;
                case sf::Keyboard::F10      :   {timeStep_ = 1.0f/30.f;} break;
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

void Application::onUpdate(float timeStep)
{
    clock_.restart();
    for(auto& navigator : navigators_)
    {
        navigator->navigate();
    }
    nav_time_ = clock_.getElapsedTime();

    clock_.restart();
    camera_.update(timeStep);
    camera_view_.setCenter(camera_.getPosition());
    camera_view_.setSize(camera_.getSize());
    for (auto& object : scene_.objects())
    {
        object->update(scene_, timeStep_);             
    }
    collision_solver_.evaluateCollisions();
    physics_time_ = clock_.getElapsedTime();
}

void Application::onRender()
{
    //TODO move this FPS limiting and counting to base class
    // otherwise values are wrong
    fpsCounter_.startMeasurement();
    fpsLimiter_.startNewFrame();

    window_.setView(camera_view_);
    tilemap_->draw(window_);
    graphics::drawtools::drawWaypoints(window_, waypoints_);
    tracks_renderer_.draw(window_);
    renderGameObjects();
    particle_system_.draw(window_);
    draw_time_ = clock_.getElapsedTime().asMilliseconds();

    if (rigid_body_debug_)
    {
        engine::RigidBodyDebugRenderer::debug(scene_, window_);
    }

    auto mousePosition = sf::Mouse::getPosition(window_);
    auto mousePositionInCamera = window_.mapPixelToCoords(mousePosition);

    window_.setView(window_.getDefaultView());
    auto mousePositionInGUI = window_.mapPixelToCoords(mousePosition);

    // Temporary hack for testing objects movement
    if (floating_button_demo_visible_)
    {
        test_floating_button_handle_->setVisibility(true);
        auto position = test_floating_button_handle_->getPosition();
        position.x += 1.0f;
        position.y = (sin(position.x / 20.f) * 100.f) + 300.f;
        if (position.x > 1000.0f) position =  sf::Vector2f(1.0f, 1.0f);
        test_floating_button_handle_->setPosition(position, gui::Alignment::LEFT);
    }
    else
    {
        test_floating_button_handle_->setVisibility(false);
    }

    bool isCurrentMouseEventLeftClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    bool isLeftMouseButtonClicked {false};

    if ((not was_last_event_left_click_) and isCurrentMouseEventLeftClicked) isLeftMouseButtonClicked = true;

    // TODO This event generation should be reworked to some state machine pattern
    // also some event queue would be nice
    // For now this hacky approach is enough for some basic concept testing
    // But looks like crap :P
    std::optional<gui::event::MouseMoved> mouseMovedEvent;
    std::optional<gui::event::MouseButtonPressed> mousePressedEvent;
    std::optional<gui::event::MouseButtonReleased> mouseReleaseEvent;

    if (last_mouse_in_gui_position_ != mousePositionInGUI)
    {
        mouseMovedEvent = gui::event::MouseMoved{.position 
            = gui::event::MousePosition{.x = mousePositionInGUI.x, .y = mousePositionInGUI.y}};
    }

    auto currentLeftClickEventStatus = gui::EventStatus{gui::EventStatus::NotConsumed};

    if (not was_last_event_left_click_ and isCurrentMouseEventLeftClicked )
    {
        mousePressedEvent = gui::event::MouseButtonPressed{
            .button = gui::event::MouseButton::Left, 
            .position = gui::event::MousePosition{.x = mousePositionInGUI.x, .y = mousePositionInGUI.y}};
    }

    if (was_last_event_left_click_ and not isCurrentMouseEventLeftClicked )
    {
        mouseReleaseEvent = gui::event::MouseButtonReleased{
            .button = gui::event::MouseButton::Left, 
            .position = gui::event::MousePosition{.x = mousePositionInGUI.x, .y = mousePositionInGUI.y}};
    }

    // FIXME: isLeftMouseButtonClicked fires only once (for gui i need proper state of mouse every frame)
    //  isLeftMouseButtonClicked is good hack for targets but for gui especially for dragging action
    //  I need to have proper mouse state every frame.
    if (mousePressedEvent) 
    {
        auto result = window_manager_->receive(mousePressedEvent.value());
        if (result == gui::EventStatus::Consumed)
        {
            currentLeftClickEventStatus = result;
        }
    }
    if (mouseReleaseEvent) window_manager_->receive(mouseReleaseEvent.value());
    
    if (mouseMovedEvent) window_manager_->receive(mouseMovedEvent.value());

    window_manager_->render(window_);

    // I'm integrating new event system in components so this code looks very messy.
    // I'll clean it when everything will switch on EventReceiver methods

    if (currentLeftClickEventStatus == gui::EventStatus::NotConsumed 
        and isLeftMouseButtonClicked)
    {
        waypoints_.emplace_back(mousePositionInCamera);               
    }

    was_last_event_left_click_ = isCurrentMouseEventLeftClicked;
    last_mouse_in_gui_position_ = mousePositionInGUI;

    clock_.restart();
    fpsLimiter_.wait();
    fpsCounter_.endMeasurement();

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

    static std::vector<GameObjectWithDistanceToCamera> objects_to_draw;
    objects_to_draw.reserve(scene_.objects().size());

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

        objects_to_draw.emplace_back(GameObjectWithDistanceToCamera{object.get(), distanceFromCameraCenter});
    }

    std::sort(objects_to_draw.begin(), objects_to_draw.end(), 
        [](const auto& left, const auto& right)
        {
            return left.distance > right.distance;
        }
    );

    for (auto& e : objects_to_draw)
    {
        e.object->draw(window_, timeStep_);
    }

    objects_to_draw.clear();
}

void Application::configureGUI()
{   
    auto quit_button = std::make_unique<gui::Button>("Quit");
    quit_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 100.f), gui::Alignment::LEFT);
    quit_button->setSize(sf::Vector2f(150.f, 30.f));
    quit_button->onClick([this](){std::cout << "Quitting...\n"; Application::close();});
    window_manager_->mainWindow()->addChild(std::move(quit_button));

    auto demo_button_1 = std::make_unique<gui::Button>("TEST");
    demo_button_1->setPosition(sf::Vector2f(100.f, 200.f), gui::Alignment::LEFT);
    demo_button_1->setSize(sf::Vector2f{200.f, 200.f});
    demo_button_1->onClick([](){std::cout << "TEST" <<std::endl;});

    test_floating_button_handle_ = demo_button_1.get();
    window_manager_->mainWindow()->addChild(std::move(demo_button_1));

    auto measurements_text = std::make_unique<gui::Label>("");
    measurements_text->setPosition(sf::Vector2f(20.f, 20.f), gui::Alignment::LEFT);
    measurements_text_handle_ = measurements_text.get();

    window_manager_->mainWindow()->addChild(std::move(measurements_text));

    auto measurements_average_text = std::make_unique<gui::Label>("");
    measurements_average_text->setPosition(sf::Vector2f(200.f, 20.f), gui::Alignment::LEFT);
    measurements_average_text_handle_ = measurements_average_text.get();

    window_manager_->mainWindow()->addChild(std::move(measurements_average_text));

    auto button = std::make_unique<gui::Button>("Help");
    button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 150.f), gui::Alignment::LEFT);
    button->setSize(sf::Vector2f(150.f, 30.f));
    button->onClick([this](){
        auto help_window = std::make_unique<game::HelpWindow>(sf::Vector2f(Config::WINDOW_WIDTH/2, 600.0f));
        help_window->setTitle("Help");
        window_manager_->addWindow(std::move(help_window));
    });
    window_manager_->mainWindow()->addChild(std::move(button));

    auto demo_button = std::make_unique<gui::Button>("BUTTON DEMO");
    demo_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 200.f), gui::Alignment::LEFT);
    demo_button->setSize(sf::Vector2f(150.f, 30.f));
    demo_button->onClick([this](){floating_button_demo_visible_ = !floating_button_demo_visible_;});
    window_manager_->mainWindow()->addChild(std::move(demo_button));

    auto spawn_window_button = std::make_unique<gui::Button>("Spawn new window");
    spawn_window_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 250.f), gui::Alignment::LEFT);
    spawn_window_button->setSize(sf::Vector2f(150.f, 30.f));
    spawn_window_button->onClick([this](){
        float random_x = rand() % 200;
        float random_y = rand() % 200;
        auto window = std::make_unique<gui::Window>(); 

        auto horizontal_layout = std::make_unique<gui::HorizontalLayout>();
        auto hello_button = std::make_unique<gui::Button>("HELLO");
        auto world_button = std::make_unique<gui::Button>("WORLD");

        hello_button->onClick([](){std::cout << "Hello?" << std::endl;});
        world_button->onClick([](){std::cout << "Is it me you looking for?" << std::endl;});

        horizontal_layout->addComponent(std::move(hello_button));
        horizontal_layout->addComponent(std::move(world_button));

        // auto horizontal_layout2 = std::make_unique<gui::HorizontalLayout>();
        // auto testing = std::make_unique<gui::Button>("TESTING");
        // auto things = std::make_unique<gui::Button>("THINGS");

        // horizontal_layout2->addComponent(std::move(testing));
        // horizontal_layout2->addComponent(std::move(things));

       // auto vertical_layout = std::make_unique<gui::VerticalLayout>();

        // vertical_layout->addComponent(std::move(horizontal_layout));
        // vertical_layout->addComponent(std::move(horizontal_layout2));

        //vertical_layout->addComponent(std::move(hello_button));
        //vertical_layout->addComponent(std::move(world_button));

        window->addComponent(std::move(horizontal_layout));
        //hello_button->setSize(sf::Vector2f{30.f,30.f});
        //window->addComponent(std::move(hello_button));

        window->setSize(sf::Vector2f(500.0f, 400.0f));
        window->setPosition(sf::Vector2f((Config::WINDOW_WIDTH+random_x)/2, 400.0f+random_y), gui::Alignment::CENTERED);
        window->setTitle("Oh my gosh");

        window_manager_->addWindow(std::move(window));
    });
    window_manager_->mainWindow()->addChild(std::move(spawn_window_button));
}

void Application::spawnSomeTanks()
{
    for (size_t i = 0; i < TANKS_COUNT; ++i)
    { 
        const auto x_spawn_position = i * 100 + 100;
        const auto y_spawn_position = x_spawn_position;
        const auto spawn_rotation = i * 36; 
        auto tank = entity::TankFactory::create(
            static_cast<entity::TankFactory::TankType>(i),
            x_spawn_position, y_spawn_position, spawn_rotation, &tracks_renderer_);

        auto navigator = std::make_unique<Navigator>(*tank, waypoints_);
        scene_.spawnObject(std::move(tank));
        navigators_.push_back(std::move(navigator));
    }
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
            const auto x_position = rand() % Config::WINDOW_WIDTH;
            const auto y_position = rand() % Config::WINDOW_HEIGHT;
            scene_.spawnObject(game::entity::TreeFactory::create(
                tree_type, x_position, y_position));
        }
    }
}

void Application::generateProfiling()
{
    // TODO: Create some proper profiler class not that
    measurements_text_handle_->setText("DRAW: " + std::to_string(draw_time_)
            + "ms\nPHYSICS: " + std::to_string(physics_time_.asMicroseconds())
            + "us\nNAV: " + std::to_string(nav_time_.asMicroseconds())
            + "us\nGUI: " + std::to_string(gui_time_.asMilliseconds()) 
            + "ms\nFPS: "+ std::to_string(fpsCounter_.getFps())
            + "\nObjects count: " + std::to_string(scene_.objects().size()));

    measurements_average_text_handle_->setText("AVG: " + std::to_string(draw_average_.calculate(draw_time_))
        + "ms\nAVG: " + std::to_string(physics_average_.calculate(physics_time_.asMicroseconds()))
        + "us\nAVG: " + std::to_string(nav_average_.calculate(nav_time_.asMicroseconds()))
        + "us\nAVG: " + std::to_string(gui_average_.calculate(gui_time_.asMilliseconds()))
        + "ms\nAVG: " + std::to_string(fps_average_.calculate(fpsCounter_.getFps())));
}

}  // namespace game
