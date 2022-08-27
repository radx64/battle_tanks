#include <cstdint>
#include <cmath>
#include <exception>
#include <iostream>
#include <vector>

#include "FontLibrary.hpp"
#include "Math.hpp"
#include "Navigator.hpp"
#include "TextureLibrary.hpp"
#include "Tank.hpp"
#include "Tilemap.hpp"

#include <SFML/Graphics.hpp>

constexpr uint32_t WINDOW_WIDTH = 800;
constexpr uint32_t WINDOW_HEIGHT = 600;
constexpr uint32_t TANKS_COUNT = 5;

constexpr double timeStep = 1.0/30.0;

void drawTarget(sf::RenderWindow& window, int x, int y)
{
    //target circle
    sf::CircleShape target(10);
    target.setFillColor(sf::Color(255, 255, 255));
    target.setOutlineThickness(2);
    target.setOutlineColor(sf::Color(250, 10, 10));
    target.setPosition(x, y);
    target.setOrigin(10,10);
    window.draw(target);

    sf::CircleShape target2(2);
    target2.setFillColor(sf::Color(250, 10, 10));
    target2.setOutlineThickness(2);
    target2.setOutlineColor(sf::Color(250, 10, 10));
    target2.setPosition(x, y);
    target2.setOrigin(2,2);
    window.draw(target2);
}

void drawWaypoints(sf::RenderWindow& window, std::vector<sf::Vector2i>& waypoints)
{
    const sf::Vector2i* last_waypoint = nullptr;
    for(const auto& waypoint : waypoints)
    {
        drawTarget(window, waypoint.x, waypoint.y);
        if(last_waypoint)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(last_waypoint->x, last_waypoint->y)),
                sf::Vertex(sf::Vector2f(waypoint.x, waypoint.y))
            };
            window.draw(line, 2, sf::Lines);
        }
        last_waypoint = &waypoint;
    }
}

int Application()
{
    try
    {
        FontLibrary::initialize();
        TextureLibrary::initialize();
        Tilemap tilemap;
        std::vector<sf::Vector2i> waypoints;
        sf::Text text;

        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Battle tanks!");
        window.setFramerateLimit(30);
        std::vector<Tank*> tanks;
        std::vector<Navigator*> navigators;
        for (uint8_t i = 0; i < TANKS_COUNT; ++i)
        {   
            const auto tank = new Tank(i, i * 100 + 100, i * 100 + 100, i * 36 + 0);
            const auto navigator = new Navigator(*tank, waypoints);
            tanks.push_back(tank);
            navigators.push_back(navigator);
        }

        int mouse_x{};
        int mouse_y{};

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    mouse_x = event.mouseButton.x;
                    mouse_y = event.mouseButton.y;

                    waypoints.emplace_back(sf::Vector2i(mouse_x, mouse_y));
                }
                else if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::C)
                    {
                        waypoints.clear();
                    }
                }
            }
            sf::Clock clock;
            //auto elapsed = clock.getElapsedTime();

            window.clear(sf::Color(0, 100, 20));
            tilemap.draw(window);
            drawWaypoints(window, waypoints);
            for (Tank* tank : tanks)
            {
                tank->draw(window);
            }
            auto draw_time = clock.getElapsedTime();

            clock.restart();
            for(Navigator* navigator : navigators)
            {
                navigator->navigate();
            }
            auto nav_time = clock.getElapsedTime();
            clock.restart();
            for (Tank* tank : tanks)
            {
                tank->physics(tanks, timeStep);
                if (tank->x_ > WINDOW_WIDTH) tank->x_ = -50;
                if (tank->y_ > WINDOW_HEIGHT) tank->y_ = -50;
                if (tank->x_ < -50) tank->x_ = WINDOW_WIDTH;
                if (tank->y_ < -50) tank->y_ = WINDOW_HEIGHT; 
            }
            auto physics_time = clock.getElapsedTime();
            text.setString("DRAW: " + std::to_string(draw_time.asMicroseconds()) 
                 + "us\nPHYSICS: " + std::to_string(physics_time.asMicroseconds())
                 + "us\nNAV: " + std::to_string(nav_time.asMicroseconds())+ "us");

            text.setFont(FontLibrary::get("armata"));
            text.setPosition(20.f, 20.f);
            text.setCharacterSize(12);
            text.setFillColor(sf::Color::Black);
            window.draw(text);
            window.display();
        }
    }
    catch (std::exception& e)
    {
        std::cout << "EXCEPTION: "<<  e.what() << std::endl;
        return -1;
    }

    return 0;
}
