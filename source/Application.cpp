#include <cstdint>
#include <cmath>
#include <exception>
#include <iostream>
#include <vector>
#include <string>

#include "Application.hpp"
#include "Camera.hpp"
#include "Context.hpp"
#include "DrawTools.hpp"
#include "FontLibrary.hpp"
#include "Math.hpp"
#include "Navigator.hpp"
#include "Particles.hpp"
#include "TextureLibrary.hpp"
#include "Tank.hpp"
#include "TankFactory.hpp"
#include "Tilemap.hpp"

#include <SFML/Graphics.hpp>

constexpr uint32_t WINDOW_WIDTH = 1920;
constexpr uint32_t WINDOW_HEIGHT = 1080;
constexpr uint32_t TANKS_COUNT = 5;

constexpr double timeStep = 1.0/30.0;

constexpr std::string_view help_text_string{
    "=== HELP ===\n"
    "WASD - moves view\n" 
    "PgUp/PgDn - zoom\n"  
    "C - clear all waypoints\n"
    "F - delete last waypoint\n"
    "T - clear tracks\n"
    "Q - quit\n"};

int Application()
{
    try
    {
        Particles particles;
        Context context;
        context.setParticles(&particles);

        const sf::Vector2f camera_initial_position{WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f};
        const sf::Vector2f camera_initial_size{WINDOW_WIDTH, WINDOW_HEIGHT};

        Camera camera(camera_initial_position, camera_initial_size);

        auto view = sf::View(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));
        view.setCenter(WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0);

        FontLibrary::initialize();
        TextureLibrary::initialize();
        Tilemap tilemap;
        std::vector<sf::Vector2i> waypoints;

        sf::Text help_text;
        help_text.setFont(FontLibrary::get("armata"));
        help_text.setCharacterSize(20);
        help_text.setFillColor(sf::Color::Black);
        help_text.setString(help_text_string.data());

        help_text.setPosition(
            WINDOW_WIDTH/2 - help_text.getGlobalBounds().width/2,
            WINDOW_HEIGHT/2 - help_text.getGlobalBounds().height/2);
        bool help_visible{false};

        sf::Text measurements_text;
        measurements_text.setFont(FontLibrary::get("armata"));
        measurements_text.setPosition(20.f, 20.f);
        measurements_text.setCharacterSize(20);
        measurements_text.setFillColor(sf::Color::Black);
        measurements_text.setOutlineColor(sf::Color(127,127,127,255));
        measurements_text.setOutlineThickness(2.f);


        sf::Text measurements_average_text;
        measurements_average_text.setFont(FontLibrary::get("armata"));
        measurements_average_text.setPosition(200.f, 20.f);
        measurements_average_text.setCharacterSize(20);
        measurements_average_text.setFillColor(sf::Color::Black);
        measurements_average_text.setOutlineColor(sf::Color(127,127,127,255));
        measurements_average_text.setOutlineThickness(2.f);

        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Battle tanks!", sf::Style::Fullscreen);
        window.setFramerateLimit(60);

        std::vector<std::unique_ptr<Tank>> tanks;
        std::vector<std::unique_ptr<Navigator>> navigators;
        for (uint8_t i = 0; i < TANKS_COUNT; ++i)
        { 
            const auto x_spawn_position = i * 100 + 100;
            const auto y_spawn_position = x_spawn_position;
            const auto spawn_rotation = i * 36; 
            auto tank = TankFactory::create_instance(static_cast<TankFactory::TankType>(i),
                x_spawn_position, y_spawn_position, spawn_rotation);

            auto navigator = std::make_unique<Navigator>(*tank, waypoints);
            tanks.push_back(std::move(tank));
            navigators.push_back(std::move(navigator));
        }

        constexpr int number_of_measures = 100;
        math::Average draw_average{number_of_measures};
        math::Average physics_average{number_of_measures};
        math::Average nav_average{number_of_measures};
        math::Average fps_average{number_of_measures};
        sf::Clock clock;
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

                        waypoints.emplace_back(worldPos);
                        break;
                    }
                    case sf::Event::KeyReleased : 
                    {
                        switch (event.key.code)
                        {
                            case sf::Keyboard::PageUp   :   camera.zoom_in(); break;
                            case sf::Keyboard::PageDown :   camera.zoom_out(); break;
                            case sf::Keyboard::C        :   waypoints.clear(); break;
                            case sf::Keyboard::T        :   Context::getParticles().clear(); break; 
                            case sf::Keyboard::W        :   camera.move(0.f,-64.f); break;
                            case sf::Keyboard::S        :   camera.move(0.f,64.f); break;
                            case sf::Keyboard::A        :   camera.move(-64.f,0.f); break;
                            case sf::Keyboard::D        :   camera.move(64.f,0.f); break;    
                            case sf::Keyboard::F        :   if(!waypoints.empty()) waypoints.pop_back(); break;
                            case sf::Keyboard::H        :   help_visible = !help_visible; break;
                            case sf::Keyboard::Q        :   window.close();
                            default                     :   {}  
                        }
                        break;
                    }
                    case sf::Event::MouseWheelMoved : 
                    {
                        if (event.mouseWheel.delta > 0) camera.zoom_in();
                        if (event.mouseWheel.delta < 0) camera.zoom_out();
                    }
                    default : {}
                }
            }
            
            camera.physics();
            view.setCenter(camera.get_position());
            view.setSize(camera.get_size());
            window.setView(view);
            window.clear(sf::Color(0, 0, 0));

            clock.restart();
            tilemap.draw(window);
            drawtools::drawWaypoints(window, waypoints);
            particles.draw(window);
            for (auto& tank : tanks)
            {
                tank->draw(window);
            }
            auto draw_time = clock.getElapsedTime();
            auto fps = 1000/draw_time.asMilliseconds();
            clock.restart();
            for(auto& navigator : navigators)
            {
                navigator->navigate();
            }
            auto nav_time = clock.getElapsedTime();
            clock.restart();
            for (auto& tank : tanks)
            {
                tank->physics(tanks, timeStep);
                // if (tank->x_ > WINDOW_WIDTH) tank->x_ = -5;
                // if (tank->y_ > WINDOW_HEIGHT) tank->y_ = -5;
                // if (tank->x_ < -5) tank->x_ = WINDOW_WIDTH;
                // if (tank->y_ < -5) tank->y_ = WINDOW_HEIGHT; 
            }

            window.setView(window.getDefaultView());

            auto physics_time = clock.getElapsedTime();
            measurements_text.setString("DRAW: " + std::to_string(draw_time.asMicroseconds())
                 + "us\nPHYSICS: " + std::to_string(physics_time.asMicroseconds())
                 + "us\nNAV: " + std::to_string(nav_time.asMicroseconds())
                 + "us\nFPS: "+ std::to_string(fps));

            window.draw(measurements_text);
            measurements_average_text.setString("AVG: " + std::to_string(draw_average.calculate(draw_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(physics_average.calculate(physics_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(nav_average.calculate(nav_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(fps_average.calculate(fps)));
            window.draw(measurements_average_text);

            if (help_visible) 
            {
                //TODO: need some GUI system later
                sf::RectangleShape text_background(sf::Vector2f(400,200));
                text_background.setFillColor(sf::Color(50, 50, 50, 50));
                text_background.setPosition(
                    help_text.getGlobalBounds().left,
                    help_text.getGlobalBounds().top);
                window.draw(text_background);
                window.draw(help_text);
            }
            // set "gameplay area view" again so mouse coordinates will be calculated properly in next mouse event
            // this can be calulated also as an offset of camera view, to not switch views back and forward
            // TODO reimplement this later 
            window.setView(view);
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
