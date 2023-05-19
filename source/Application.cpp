#include <cstdint>
#include <cmath>
#include <exception>
#include <iostream>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "Application.hpp"
#include "DrawTools.hpp"
#include "FontLibrary.hpp"
#include "Math.hpp"
#include "TextureLibrary.hpp"
#include "TankFactory.hpp"

constexpr uint32_t WINDOW_WIDTH = 1920;
constexpr uint32_t WINDOW_HEIGHT = 1080;
constexpr uint32_t TANKS_COUNT = 5;

constexpr double timeStep = 1.0/30.0;

constexpr std::string_view help_text_string{
    "=== HELP ===\n"
    "WASD - moves view_\n" 
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
    FontLibrary::initialize();
    TextureLibrary::initialize();
    tilemap_ = std::make_unique<Tilemap>();

    configureTexts();
}


void Application::configureTexts()
{
    const auto& current_font = FontLibrary::get("armata");

    help_text_.setFont(current_font);
    help_text_.setCharacterSize(20);
    help_text_.setFillColor(sf::Color::Black);
    help_text_.setString(help_text_string.data());

    help_text_.setPosition(
        WINDOW_WIDTH/2 - help_text_.getGlobalBounds().width/2,
        WINDOW_HEIGHT/2 - help_text_.getGlobalBounds().height/2);

    measurements_text_.setFont(current_font);
    measurements_text_.setPosition(20.f, 20.f);
    measurements_text_.setCharacterSize(20);
    measurements_text_.setFillColor(sf::Color::Black);
    measurements_text_.setOutlineColor(sf::Color(127,127,127,255));
    measurements_text_.setOutlineThickness(2.f);

    measurements_average_text_.setFont(current_font);
    measurements_average_text_.setPosition(200.f, 20.f);
    measurements_average_text_.setCharacterSize(20);
    measurements_average_text_.setFillColor(sf::Color::Black);
    measurements_average_text_.setOutlineColor(sf::Color(127,127,127,255));
    measurements_average_text_.setOutlineThickness(2.f);
}

void Application::spawnSomeTanks()
{
    for (uint32_t i = 0; i < TANKS_COUNT; ++i)
    { 
        const auto x_spawn_position = i * 100 + 100;
        const auto y_spawn_position = x_spawn_position;
        const auto spawn_rotation = i * 36; 
        auto tank = TankFactory::create_instance(static_cast<TankFactory::TankType>(i),
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
        
        bool help_visible{false};

        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Battle tanks!", sf::Style::Fullscreen);
        window.setFramerateLimit(60);


        constexpr int number_of_measurements = 100;
        math::Average draw_average{number_of_measurements};
        math::Average physics_average{number_of_measurements};
        math::Average nav_average{number_of_measurements};
        math::Average fps_average{number_of_measurements};
        sf::Clock clock;

        bool debug_mode{false};
        Tank::set_debug(debug_mode);

        spawnSomeTanks();

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed : {window.close(); break;}
                    case sf::Event::MouseButtonPressed : 
                    {
                        // Get the cursor position in view coordinates
                        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                        waypoints_.emplace_back(worldPos);
                        break;
                    }
                    case sf::Event::KeyReleased : 
                    {
                        switch (event.key.code)
                        {
                            case sf::Keyboard::PageUp   :   camera_.zoom_in(); break;
                            case sf::Keyboard::PageDown :   camera_.zoom_out(); break;
                            case sf::Keyboard::C        :   waypoints_.clear(); break;
                            case sf::Keyboard::F12      :   {debug_mode=!debug_mode; Tank::set_debug(debug_mode);} break;
                            case sf::Keyboard::T        :   Context::getParticles().clear(); break;
                            case sf::Keyboard::F        :   if(!waypoints_.empty()) waypoints_.pop_back(); break;
                            case sf::Keyboard::H        :   help_visible = !help_visible; break;
                            case sf::Keyboard::Q        :   window.close();
                            default                     :   {}  
                        }
                        break;
                    }

                    case sf::Event::MouseWheelMoved : 
                    {
                        if (event.mouseWheel.delta > 0) camera_.zoom_in(event.mouseWheel.x, event.mouseWheel.y);
                        if (event.mouseWheel.delta < 0) camera_.zoom_out();
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
            view_.setCenter(camera_.get_position());
            view_.setSize(camera_.get_size());
            window.setView(view_);
            window.clear(sf::Color(0, 0, 0));

            clock.restart();
            tilemap_->draw(window);
            drawtools::drawWaypoints(window, waypoints_);
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
            measurements_text_.setString("DRAW: " + std::to_string(draw_time)
                 + "ms\nPHYSICS: " + std::to_string(physics_time.asMicroseconds())
                 + "us\nNAV: " + std::to_string(nav_time.asMicroseconds())
                 + "us\nFPS: "+ std::to_string(fps));

            window.draw(measurements_text_);
            measurements_average_text_.setString("AVG: " + std::to_string(draw_average.calculate(draw_time))
                + "ms\nAVG: " + std::to_string(physics_average.calculate(physics_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(nav_average.calculate(nav_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(fps_average.calculate(fps)));
            window.draw(measurements_average_text_);

            if (help_visible) 
            {
                //TODO: need some GUI system later
                sf::RectangleShape text_background(sf::Vector2f(400,200));
                text_background.setFillColor(sf::Color(50, 50, 50, 50));
                text_background.setPosition(
                    help_text_.getGlobalBounds().left,
                    help_text_.getGlobalBounds().top);
                window.draw(text_background);
                window.draw(help_text_);
            }
            // set "gameplay area view_" again so mouse coordinates will be calculated properly in next mouse event
            // this can be calulated also as an offset of camera view_, to not switch view_s back and forward
            // TODO reimplement this later if needed, delete this todo otherwise
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
