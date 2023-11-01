#include <cstdint>
#include <cmath>
#include <exception>
#include <iostream>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "game/Application.hpp"
#include "game/TankFactory.hpp"
#include "gui/Button.hpp"
#include "gui/Label.hpp"
#include "gui/Window.hpp"
#include "graphics/DrawTools.hpp"
#include "graphics/TextureLibrary.hpp"
#include "math/Math.hpp"

namespace game 
{

constexpr uint32_t WINDOW_WIDTH = 1920;
constexpr uint32_t WINDOW_HEIGHT = 1080;
constexpr uint32_t TANKS_COUNT = 5;

constexpr double timeStep = 1.0/30.0;

constexpr std::string_view help_text_string{
    "=== HELP ===\n"
    "WASD - moves view\n" 
    "PgUp/PgDn - zoom\n"  
    "C - clear all waypoints\n"
    "F12 - toggle debug draw mode\n"
    "F - delete last waypoint\n"
    "T - clear tracks\n"
    "Q - quit\n"};


Application::Application()
: camera_initial_position_{WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f}
, camera_initial_size_{WINDOW_WIDTH, WINDOW_HEIGHT}
, camera_{camera_initial_position_, camera_initial_size_}
, view_{sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT)}
{
    context_.setParticles(&particles_);
    gui::FontLibrary::initialize();
    graphics::TextureLibrary::initialize();
    tilemap_ = std::make_unique<graphics::Tilemap>();

    window_manager_ = std::make_unique<gui::WindowManager>();

    configureTexts();
}

void Application::configureTexts()
{
    auto parent_label = std::make_unique<gui::Label>("PARENT_LABEL");
    parent_label->setPosition(sf::Vector2f(100.0f, 200.0f), gui::Alignment::LEFT);

    auto child_label_1 = new gui::Label("CHILD_LABEL1", parent_label.get());
    child_label_1->setPosition(sf::Vector2f(0.0f, 30.0f), gui::Alignment::RIGHT);  

    auto child_label_2 = new gui::Label("CHILD_LABEL2", parent_label.get());
    child_label_2->setPosition(sf::Vector2f(0.0f, 30.0f), gui::Alignment::LEFT);  

    auto second_level_child_label = new gui::Label("2ND_LEVEL_CHILD_LABEL", child_label_2);
    second_level_child_label->setPosition(sf::Vector2f(0.0f, 30.0f), gui::Alignment::CENTERED);  

    guiElements_.push_back(std::move(parent_label));

    auto measurements_text = std::make_unique<gui::Label>("");
    measurements_text->setPosition(sf::Vector2f(20.f, 20.f), gui::Alignment::LEFT);
    measurements_text_handle_ = measurements_text.get();
    guiElements_.push_back(std::move(measurements_text));

    auto measurements_average_text = std::make_unique<gui::Label>("");
    measurements_average_text->setPosition(sf::Vector2f(200.f, 20.f), gui::Alignment::LEFT);
    measurements_average_text_handle_ = measurements_average_text.get();
    guiElements_.push_back(std::move(measurements_average_text));

    auto help_window = std::make_unique<gui::Window>();
    help_window->setSize(sf::Vector2f(500.0f, 400.0f));
    help_window->setPosition(sf::Vector2f(WINDOW_WIDTH/2, 200.0f), gui::Alignment::CENTERED);
    help_window_handle_ = help_window.get();

    auto help_text = new gui::Label(help_text_string.data(), help_window.get());
    help_text->setPosition(sf::Vector2f(20.0f, 20.0f), gui::Alignment::LEFT);

    window_manager_->addWindow(std::move(help_window));

    auto second_window = std::make_unique<gui::Window>(); 
    second_window->setSize(sf::Vector2f(500.0f, 400.0f));
    second_window->setPosition(sf::Vector2f((WINDOW_WIDTH-100)/2, 300.0f), gui::Alignment::CENTERED);

    auto second_window_close_button = new gui::Button(second_window.get(), "[X]");
    second_window_close_button->setPosition(sf::Vector2f(5.f, 5.f), gui::Alignment::LEFT);
    second_window_close_button->setSize(sf::Vector2f(30.f, 30.f));
    second_window_close_button->onClick([window = second_window.get()](){window->close();});

    window_manager_->addWindow(std::move(second_window));

    auto third_window = std::make_unique<gui::Window>(); 
    third_window->setSize(sf::Vector2f(500.0f, 400.0f));
    third_window->setPosition(sf::Vector2f((WINDOW_WIDTH+100)/2, 400.0f), gui::Alignment::CENTERED);

    auto third_window_close_button = new gui::Button(third_window.get(), "[X]");
    third_window_close_button->setPosition(sf::Vector2f(5.f, 5.f), gui::Alignment::LEFT);
    third_window_close_button->setSize(sf::Vector2f(30.f, 30.f));
    third_window_close_button->onClick([window = third_window.get()](){window->close();});

    window_manager_->addWindow(std::move(third_window));

    auto button = std::make_unique<gui::Button>();
    button->setPosition(sf::Vector2f(WINDOW_WIDTH - 200.f, 200.f), gui::Alignment::LEFT);
    button->setSize(sf::Vector2f(150.f, 50.f));
    button->setText("Help!");
    button->onClick([this](){help_visible_ = !help_visible_;});
    guiElements_.push_back(std::move(button));

    auto demo_label_button = std::make_unique<gui::Button>();
    demo_label_button->setPosition(sf::Vector2f(WINDOW_WIDTH - 200.f, 300.f), gui::Alignment::LEFT);
    demo_label_button->setSize(sf::Vector2f(150.f, 50.f));
    demo_label_button->setText("LABEL DEMO!");
    demo_label_button->onClick([this](){label_demo_visible_ = !label_demo_visible_;});
    guiElements_.push_back(std::move(demo_label_button));
}

void Application::spawnSomeTanks()
{
    for (uint32_t i = 0; i < TANKS_COUNT; ++i)
    { 
        const auto x_spawn_position = i * 100 + 100;
        const auto y_spawn_position = x_spawn_position;
        const auto spawn_rotation = i * 36; 
        auto tank = TankFactory::createInstance(static_cast<TankFactory::TankType>(i),
            x_spawn_position, y_spawn_position, spawn_rotation);

        auto navigator = std::make_unique<Navigator>(*tank, waypoints_);
        tanks_.push_back(std::move(tank));
        navigators_.push_back(std::move(navigator));
    }
}

int Application::run()
{
    try
    {
        // TODO: move those member creations to class fields
        view_.setCenter(WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0);

        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Battle tanks!", sf::Style::Fullscreen);
        window.setFramerateLimit(60);

        constexpr int number_of_measurements = 100;
        math::Average draw_average{number_of_measurements};
        math::Average physics_average{number_of_measurements};
        math::Average nav_average{number_of_measurements};
        math::Average fps_average{number_of_measurements};
        sf::Clock clock;

        bool debug_mode{false};
        Tank::setDebug(debug_mode);

        spawnSomeTanks();

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed : {window.close(); break;}
                    case sf::Event::KeyReleased : 
                    {
                        switch (event.key.code)
                        {
                            case sf::Keyboard::PageUp   :   camera_.zoomIn(); break;
                            case sf::Keyboard::PageDown :   camera_.zoomOut(); break;
                            case sf::Keyboard::C        :   waypoints_.clear(); break;
                            case sf::Keyboard::F12      :   {debug_mode=!debug_mode; Tank::setDebug(debug_mode);} break;
                            case sf::Keyboard::T        :   Context::getParticles().clear(); break;
                            case sf::Keyboard::F        :   if(!waypoints_.empty()) waypoints_.pop_back(); break;
                            case sf::Keyboard::H        :   help_visible_ = !help_visible_; break;
                            case sf::Keyboard::Q        :   window.close();
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

            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (mousePos.x < 10) {camera_.move(-20.f,0.f);}
            if ((uint32_t)mousePos.x > WINDOW_WIDTH - 10) {camera_.move(20.f,0.f);}
            if (mousePos.y < 10) {camera_.move(0.f,-20.f);}
            if ((uint32_t)mousePos.y > WINDOW_HEIGHT - 10) {camera_.move(0.f,20.f);}

            camera_.physics();
            view_.setCenter(camera_.getPosition());
            view_.setSize(camera_.getSize());
            window.setView(view_);
            window.clear(sf::Color(0, 0, 0));

            clock.restart();
            tilemap_->draw(window);
            graphics::drawtools::drawWaypoints(window, waypoints_);
            particles_.draw(window);
            for (auto& tank : tanks_)
            {
                tank->draw(window);
            }
            auto draw_time = clock.getElapsedTime().asMilliseconds();
            uint32_t fps{};

            if (draw_time > 1)
            {
                fps = 1000.0/static_cast<double>(draw_time);
            }
            else
            {
                fps = 1000;
            }
            clock.restart();
            for(auto& navigator : navigators_)
            {
                navigator->navigate();
            }
            auto nav_time = clock.getElapsedTime();
            clock.restart();
            for (auto& tank : tanks_) tank->physics(tanks_, timeStep);

            window.setView(window.getDefaultView());

            auto physics_time = clock.getElapsedTime();
            measurements_text_handle_->setText("DRAW: " + std::to_string(draw_time)
                 + "ms\nPHYSICS: " + std::to_string(physics_time.asMicroseconds())
                 + "us\nNAV: " + std::to_string(nav_time.asMicroseconds())
                 + "us\nFPS: "+ std::to_string(fps));

            measurements_average_text_handle_->setText("AVG: " + std::to_string(draw_average.calculate(draw_time))
                + "ms\nAVG: " + std::to_string(physics_average.calculate(physics_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(nav_average.calculate(nav_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(fps_average.calculate(fps)));


            help_window_handle_->setVisibility(help_visible_);

            // set "gameplay area view_" again so mouse coordinates will be calculated properly in next mouse event
            // this can be calulated also as an offset of camera view_, to not switch view_s back and forward
            // TODO reimplement this later if needed, delete this todo otherwise

            // Temporary hack for testing objects movement
            if (label_demo_visible_)
            {
                guiElements_[0]->setVisibility(true);
                auto position = guiElements_[0]->getPosition();
                position += sf::Vector2f(1.0f, 1.0f);
                if (position.x > 700.0f) position =  sf::Vector2f(1.0f, 1.0f);
                guiElements_[0]->setPosition(position, gui::Alignment::LEFT);
            }
            else
            {
                guiElements_[0]->setVisibility(false);
            }

            bool wasMouseEventCapturedByGuiSubsystem {false};

            bool isLeftMouseButtonClicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

            for (auto& guiElement : guiElements_)
            {
                if (guiElement->update(mousePosition, isLeftMouseButtonClicked))
                {
                    wasMouseEventCapturedByGuiSubsystem = true;
                }
                guiElement->render(window);
            }

            if (window_manager_->update(mousePosition, isLeftMouseButtonClicked))
            {
                wasMouseEventCapturedByGuiSubsystem = true;
            }    
            window_manager_->render(window);

            if (!wasMouseEventCapturedByGuiSubsystem && isLeftMouseButtonClicked)
            {

                // Get the cursor position in view coordinates
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                waypoints_.emplace_back(worldPos);               
            }

            window.setView(view_);
            window.display();
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
