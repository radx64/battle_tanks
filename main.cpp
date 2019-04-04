#include <cstdint>
#include <cmath>
#include <iostream>
#include "source/Tank.hpp"

#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Battle tanks DEMO!");
    window.setFramerateLimit(60);
    sf::Clock clock;
    std::vector<Tank*> tanks;
    for (uint8_t i = 1; i < 10; ++i)
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

        for (Tank* tank : tanks)
        {
            float direction = atan2(mouse_y - tank->y_, mouse_x - tank->x_);
            tank->set_throtle(1.0f);
            tank->set_direction(direction * 180 / M_PI);
            tank->physics();
            tank->draw(window);
            if (tank->x_ > 1100) tank->x_ = -50;
            if (tank->y_ > 800) tank->y_ = -50;
            if (tank->x_ < -50) tank->x_ = 1100;
            if (tank->y_ < -50) tank->y_ = 800;
            
        }
        
        //click circle
        sf::CircleShape shape(10);
        shape.setFillColor(sf::Color(0, 0, 0));
        shape.setOutlineThickness(5);
        shape.setOutlineColor(sf::Color(250, 10, 10));
        shape.setPosition(mouse_x, mouse_y);
        window.draw(shape);
        window.display();
    }

    return 0;
}
