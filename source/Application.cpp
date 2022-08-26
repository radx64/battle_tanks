#include <cstdint>
#include <cmath>
#include <exception>
#include <iostream>

#include "FontLibrary.hpp"
#include "Math.hpp"
#include "TextureLibrary.hpp"
#include "Tank.hpp"
#include "Tilemap.hpp"

#include <SFML/Graphics.hpp>

constexpr uint32_t WINDOW_WIDTH = 800;
constexpr uint32_t WINDOW_HEIGHT = 600;
constexpr uint32_t TANKS_COUNT = 5;

constexpr double timeStep = 1.0/60.0;


void drawTarget(sf::RenderWindow& window, int x, int y)
{
    //target circle
    sf::CircleShape target(15);
    target.setFillColor(sf::Color(255, 255, 255));
    target.setOutlineThickness(5);
    target.setOutlineColor(sf::Color(250, 10, 10));
    target.setPosition(x, y);
    target.setOrigin(15,15);
    window.draw(target);

    sf::CircleShape target2(5);
    target2.setFillColor(sf::Color(250, 10, 10));
    target2.setOutlineThickness(5);
    target2.setOutlineColor(sf::Color(250, 10, 10));
    target2.setPosition(x, y);
    target2.setOrigin(5,5);
    window.draw(target2);
}


int Application()
{
    try
    {
        FontLibrary::initialize();
        TextureLibrary::initialize();
        Tilemap tilemap;

        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Battle tanks!");
        window.setFramerateLimit(60);
        std::vector<Tank*> tanks;
        for (uint8_t i = 0; i < TANKS_COUNT; ++i)
            tanks.push_back(new Tank(i, i * 100 + 100, i * 100 + 100, i * 36 + 0));

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
                }
            }

            //sf::Clock clock;
            //auto elapsed = clock.getElapsedTime();
            window.clear(sf::Color(0, 100, 20));
            tilemap.draw(window);

            drawTarget(window, mouse_x, mouse_y);

            for (Tank* tank : tanks)
            {
                double dist = math::distance(mouse_x, mouse_y, tank->x_, tank->y_);

                if (dist > 1.0f)
                {
                    double direction = atan2((double)mouse_y - tank->y_, (double)mouse_x - tank->x_);
                    tank->set_throtle(std::min(1.0, dist*0.02));
                    tank->set_direction(direction * 180.0 / M_PI);
                }
                else
                {
                    tank->set_throtle(0.0f);
                }

                tank->physics(tanks, timeStep);
                tank->draw(window);
                if (tank->x_ > WINDOW_WIDTH) tank->x_ = -50;
                if (tank->y_ > WINDOW_HEIGHT) tank->y_ = -50;
                if (tank->x_ < -50) tank->x_ = WINDOW_WIDTH;
                if (tank->y_ < -50) tank->y_ = WINDOW_HEIGHT;
                
            }
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
