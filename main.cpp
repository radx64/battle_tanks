#include <cstdint>
#include <cmath>
#include <exception>
#include <iostream>

#include "source/FontLibrary.hpp"
#include "source/TextureLibrary.hpp"
#include "source/Tank.hpp"
#include "source/Tilemap.hpp"

#include <SFML/Graphics.hpp>

float distance(float x1, float y1, float x2, float y2)
{
    return sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

int main()
{
    try
    {
        FontLibrary::initialize();
        TextureLibrary::initialize();
        Tilemap tilemap;

        sf::RenderWindow window(sf::VideoMode(800, 600), "Battle tanks!");
        window.setFramerateLimit(60);
        sf::Clock clock;
        std::vector<Tank*> tanks;
        for (uint8_t i = 1; i < 2; ++i)
            tanks.push_back(new Tank(i * 50, i * 50, i * 36 + 0));

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
                float dist = distance(mouse_x, mouse_y, tank->x_, tank->y_);

                if (dist > 10.0f)
                {
                    float direction = atan2(mouse_y - tank->y_, mouse_x - tank->x_);
                    tank->set_throtle(1.0f);
                    tank->set_direction(direction * 180 / M_PI);
                }
                else
                {
                    tank->set_throtle(0.f);
                }

                tank->physics();
                tank->draw(window);
                if (tank->x_ > 1100) tank->x_ = -50;
                if (tank->y_ > 800) tank->y_ = -50;
                if (tank->x_ < -50) tank->x_ = 1100;
                if (tank->y_ < -50) tank->y_ = 800;
                
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
