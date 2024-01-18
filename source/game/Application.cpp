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
#include "game/RigidBodyDebugRenderer.hpp"
#include "graphics/DrawTools.hpp"
#include "graphics/TextureLibrary.hpp"
#include "gui/Button.hpp"
#include "gui/Event.hpp"
#include "gui/Label.hpp"
#include "gui/Layout.hpp"
#include "gui/Window.hpp"
#include "math/Math.hpp"

namespace game 
{

constexpr uint32_t WINDOW_WIDTH = 1920;
constexpr uint32_t WINDOW_HEIGHT = 1000;
constexpr size_t TANKS_COUNT = 5;
constexpr size_t BARRELS_COUNT = 10;
constexpr size_t CRATES_COUNT = 10;

Application::Application()
: camera_initial_position_{WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f}
, camera_initial_size_{WINDOW_WIDTH, WINDOW_HEIGHT}
, camera_{camera_initial_position_, camera_initial_size_}
, camera_view_{sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT)}
, window_(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "Battle tanks!")
, collision_solver_(scene_)
{
    context_.setParticleSystem(&particleSystem_);
    context_.setScene(&scene_);
    context_.setCamera(&camera_);
    gui::FontLibrary::initialize();
    graphics::TextureLibrary::initialize();
    tilemap_ = std::make_unique<graphics::Tilemap>();
    window_manager_ = std::make_unique<gui::WindowManager>(sf::Vector2f{WINDOW_WIDTH, WINDOW_HEIGHT});
    auto desktop = sf::VideoMode::getDesktopMode();
    window_.setPosition(sf::Vector2i(desktop.width/2 - window_.getSize().x/2, desktop.height/2 - window_.getSize().y/2));
    camera_view_.setCenter(WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0);

    configureGUI();
}

void Application::configureGUI()
{   
    auto quit_button = std::make_unique<gui::Button>("Quit");
    quit_button->setPosition(sf::Vector2f(WINDOW_WIDTH - 200.f, 100.f), gui::Alignment::LEFT);
    quit_button->setSize(sf::Vector2f(150.f, 50.f));
    quit_button->onClick([this](){std::cout << "Quitting...\n"; window_.close();});
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
    button->setPosition(sf::Vector2f(WINDOW_WIDTH - 200.f, 200.f), gui::Alignment::LEFT);
    button->setSize(sf::Vector2f(150.f, 50.f));
    button->onClick([this](){
        auto help_window = std::make_unique<game::HelpWindow>(sf::Vector2f(WINDOW_WIDTH/2, 600.0f));
        help_window->setTitle("Help");
        window_manager_->addWindow(std::move(help_window));
    });
    window_manager_->mainWindow()->addChild(std::move(button));

    auto demo_button = std::make_unique<gui::Button>("BUTTON DEMO");
    demo_button->setPosition(sf::Vector2f(WINDOW_WIDTH - 200.f, 300.f), gui::Alignment::LEFT);
    demo_button->setSize(sf::Vector2f(150.f, 50.f));
    demo_button->onClick([this](){floating_button_demo_visible_ = !floating_button_demo_visible_;});
    window_manager_->mainWindow()->addChild(std::move(demo_button));

    auto spawn_window_button = std::make_unique<gui::Button>("Spawn new window");
    spawn_window_button->setPosition(sf::Vector2f(WINDOW_WIDTH - 200.f, 400.f), gui::Alignment::LEFT);
    spawn_window_button->setSize(sf::Vector2f(150.f, 50.f));
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
        window->setPosition(sf::Vector2f((WINDOW_WIDTH+random_x)/2, 400.0f+random_y), gui::Alignment::CENTERED);
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
            x_spawn_position, y_spawn_position, spawn_rotation);

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
            const auto x_position = rand() % WINDOW_WIDTH;
            const auto y_position = rand() % WINDOW_HEIGHT;
            scene_.spawnObject(game::entity::TreeFactory::create(
                tree_type, x_position, y_position));
        }
    }
}

int Application::run()
{
    try
    {
        // TODO: move those member creations to class fields
        // And create some proper profiler class not that
        constexpr int number_of_measurements = 10;
        math::Average draw_average{number_of_measurements};
        math::Average physics_average{number_of_measurements};
        math::Average nav_average{number_of_measurements};
        math::Average fps_average{number_of_measurements};
        math::Average gui_average{number_of_measurements};

        sf::Clock clock;

        bool tank_debug_mode{false};
        entity::Tank::setDebug(tank_debug_mode);

        spawnSomeTanks();
        spawnSomeBarrelsAndCratesAndTress();

        while (window_.isOpen())
        {
            scene_.update();
            particleSystem_.update(timeStep_);

            fpsLimiter_.startNewFrame();
            fpsCounter_.startMeasurement();
            sf::Event event;
            while (window_.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed : { window_.close(); break; } 
                    case sf::Event::KeyReleased : 
                    {
                        switch (event.key.code)
                        {
                            case sf::Keyboard::PageUp   :   camera_.zoomIn(); break;
                            case sf::Keyboard::PageDown :   camera_.zoomOut(); break;
                            case sf::Keyboard::C        :   waypoints_.clear(); break;
                            case sf::Keyboard::F8       :   {timeStep_ = 1.0f/300.f;} break;
                            case sf::Keyboard::F9       :   {timeStep_ = 1.0f/150.f;} break;
                            case sf::Keyboard::F10      :   {timeStep_ = 1.0f/30.f;} break;
                            case sf::Keyboard::F11      :   {rigid_body_debug_ = !rigid_body_debug_;} break;
                            case sf::Keyboard::F12      :   {tank_debug_mode=!tank_debug_mode; entity::Tank::setDebug(tank_debug_mode);} break;
                            case sf::Keyboard::T        :   Context::getParticleSystem().clear(); break;
                            case sf::Keyboard::F        :   if(!waypoints_.empty()) waypoints_.pop_back(); break;
                            case sf::Keyboard::Q        :   window_.close();
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
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) camera_.move(0.f,-20.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) camera_.move(0.f,20.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) camera_.move(-20.f,0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) camera_.move(20.f,0.f);

            camera_.update();
            camera_view_.setCenter(camera_.getPosition());
            camera_view_.setSize(camera_.getSize());

            window_.setView(camera_view_);
            window_.clear(sf::Color(0, 0, 0));

            auto mousePosition = sf::Mouse::getPosition(window_);
            auto mousePositionInCamera = window_.mapPixelToCoords(mousePosition);
            
            if (mousePosition.x < 10) {camera_.move(-20.f,0.f);}
            if ((uint32_t)mousePosition.x > WINDOW_WIDTH - 10) {camera_.move(20.f,0.f);}
            if (mousePosition.y < 10) {camera_.move(0.f,-20.f);}
            if ((uint32_t)mousePosition.y > WINDOW_HEIGHT - 10) {camera_.move(0.f,20.f);}

            tilemap_->draw(window_);
            graphics::drawtools::drawWaypoints(window_, waypoints_);
            particleSystem_.drawTracks(window_);

            for (auto& object : scene_.objects())
            {
                if (object.get() != nullptr)
                {
                    object->draw(window_, timeStep_);
                } 
            }

            particleSystem_.draw(window_);

            if (rigid_body_debug_)
            {
                game::RigidBodyDebugRenderer::debug(scene_, window_);
            }

            auto draw_time = clock.getElapsedTime().asMilliseconds();
            clock.restart();
            for(auto& navigator : navigators_)
            {
                navigator->navigate();
            }
            auto nav_time = clock.getElapsedTime();
            clock.restart();

            // TODO I can't use references here as in update method
            // clients can add objects to scene
            // and vector reealoc can mess those references
            // so for now I keeping pointers but in a future maybe 
            // objects to be "created" should be stored separately
            // and then added to scene at the end of a game loop iteration

            const auto& scene_objects = scene_.objects();

            for (size_t index = 0; index < scene_objects.size(); ++index )
            {
                GameObject* object = scene_objects[index].get();
                if (object != nullptr)
                {
                    object->update(scene_, timeStep_);
                }             
            }

            collision_solver_.evaluateCollisions();

            auto physics_time = clock.getElapsedTime();

            window_.setView(window_.getDefaultView());

            auto mousePositionInGUI = window_.mapPixelToCoords(mousePosition);

            // set "gameplay area camera_view_" again so mouse coordinates will be calculated properly in next mouse event
            // this can be calulated also as an offset of camera camera_view_, to not switch view_s back and forward
            // TODO reimplement this later if needed, delete this todo otherwise
            
            clock.restart();
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

            auto gui_time = clock.getElapsedTime();
            
            // I'm integrating new event system in components so this code looks very messy.
            // I'll clean it when everything will switch on EventReceiver methods
            if (currentLeftClickEventStatus == gui::EventStatus::NotConsumed 
                and isLeftMouseButtonClicked)
            {
                waypoints_.emplace_back(mousePositionInCamera);               
            }

            was_last_event_left_click_ = isCurrentMouseEventLeftClicked;
            last_mouse_in_gui_position_ = mousePositionInGUI;

            measurements_text_handle_->setText("DRAW: " + std::to_string(draw_time)
                 + "ms\nPHYSICS: " + std::to_string(physics_time.asMicroseconds())
                 + "us\nNAV: " + std::to_string(nav_time.asMicroseconds())
                 + "us\nGUI: " + std::to_string(gui_time.asMilliseconds()) 
                 + "ms\nFPS: "+ std::to_string(fpsCounter_.getFps())
                 + "\nObjects count: " + std::to_string(scene_.objects().size()));

            measurements_average_text_handle_->setText("AVG: " + std::to_string(draw_average.calculate(draw_time))
                + "ms\nAVG: " + std::to_string(physics_average.calculate(physics_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(nav_average.calculate(nav_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(gui_average.calculate(gui_time.asMilliseconds()))
                + "ms\nAVG: " + std::to_string(fps_average.calculate(fpsCounter_.getFps())));

            window_.display();
            fpsLimiter_.wait();
            fpsCounter_.endMeasurement();
        }
    }
    catch (std::exception& e)
    {
        std::cout << "EXCEPTION THROWN: "<<  e.what() << std::endl;
        return -1;
    }

    return 0;
}

}  // namespace game
