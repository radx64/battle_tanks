#include <cstdint>
#include <cmath>
#include <exception>
#include <iostream>

#include "source/FontLibrary.hpp"
#include "source/TextureLibrary.hpp"
#include "source/Tank.hpp"
#include "source/Tilemap.hpp"

#include <SFML/Graphics.hpp>

double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

constexpr uint32_t WINDOW_WIDTH = 800;
constexpr uint32_t WINDOW_HEIGHT = 600;

constexpr uint32_t TANKS_COUNT = 4;

int main()
{
    try
    {
        FontLibrary::initialize();
        TextureLibrary::initialize();
        Tilemap tilemap;

        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Battle tanks!");
        window.setFramerateLimit(60);
        sf::Clock clock;
        std::vector<Tank*> tanks;
        for (uint8_t i = 0; i < TANKS_COUNT; ++i)
            tanks.push_back(new Tank(i, i * 100 + 100, i * 100 + 100, i * 36 + 0));

        float mouse_x = 300;
        float mouse_y = 300;

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

            //auto elapsed = clock.getElapsedTime();
            (void) clock;
            window.clear(sf::Color(0, 100, 20));
            tilemap.draw(window);

            //target circle
            sf::CircleShape target(10);
            target.setFillColor(sf::Color(0, 0, 0));
            target.setOutlineThickness(5);
            target.setOutlineColor(sf::Color(250, 10, 10));
            target.setPosition(mouse_x, mouse_y);
            target.setOrigin(10,10);
            window.draw(target);

            for (Tank* tank : tanks)
            {
                double dist = distance(mouse_x, mouse_y, tank->x_, tank->y_);

                if (dist > 20.0f)
                {
                    double direction = atan2((double)mouse_y - tank->y_, (double)mouse_x - tank->x_);
                    tank->set_throtle(std::min(1.0, dist*0.02));
                    tank->set_direction(direction * 180.0 / M_PI);
                }
                else
                {
                    tank->set_throtle(0.0f);
                }
                
                tank->physics(tanks);
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
        std::cout << "EXCPTIN: "<<  e.what() << std::endl;
        return -1;
    }

    return 0;
}
