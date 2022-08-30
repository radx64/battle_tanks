#include <cstdint>
#include <cmath>
#include <exception>
#include <iostream>
#include <vector>
#include <array>
#include <numeric>

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

template<int window_size>
class Average
{
public:
int calculate(int new_value)
{
    measurements_[current_index_] = new_value;
    current_index_++;
    if (current_index_ >= window_size) current_index_=0;

    return std::accumulate(measurements_.begin(), measurements_.end(), 0) / window_size;
}
protected:
    std::array<int, window_size> measurements_{};
    size_t current_index_{};
};


void drawLine(sf::RenderWindow& window, int x1, int y1, int x2, int y2, sf::Color color)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1, y1), color),
        sf::Vertex(sf::Vector2f(x2, y2), color)
    };

    window.draw(line, 2, sf::Lines);    
}

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
            drawLine(window, last_waypoint->x, last_waypoint->y, waypoint.x, waypoint.y, sf::Color::White);
        }
        last_waypoint = &waypoint;
    }

    if (waypoints.size() > 2) 
    {
        drawLine(window, waypoints.front().x, 
            waypoints.front().y, 
            waypoints.back().x, 
            waypoints.back().y,
            sf::Color::Red);
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

        text.setFont(FontLibrary::get("glassTTY"));
        text.setPosition(20.f, 20.f);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);

        sf::Text text2;
        text2.setFont(FontLibrary::get("glassTTY"));
        text2.setPosition(160.f, 20.f);
        text2.setCharacterSize(20);
        text2.setFillColor(sf::Color::Black);

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

        Average<100> draw_average{};
        Average<100> physics_average{};
        Average<100> nav_average{};
        sf::Clock clock;
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
                    if (event.key.code == sf::Keyboard::D)
                    {
                        if(!waypoints.empty()) waypoints.pop_back();
                    }
                }
            }

            window.clear(sf::Color(0, 0, 0));

            clock.restart();
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
                 + "us\nNAV: " + std::to_string(nav_time.asMicroseconds())
                 + "us");

            window.draw(text);
            text2.setString("AVG: " + std::to_string(draw_average.calculate(draw_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(physics_average.calculate(physics_time.asMicroseconds()))
                + "us\nAVG: " + std::to_string(nav_average.calculate(nav_time.asMicroseconds()))
                + "us");
            window.draw(text2);
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
