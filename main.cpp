#include <cstdint>

#include "source/Tank.hpp"

#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Battle tanks DEMO!");
    window.setFramerateLimit(60);
    sf::Clock clock;
    std::vector<Tank*> tanks;
    for (uint8_t i = 0; i < 10; ++i)
    {
        tanks.push_back(new Tank(i * 100, i * 100, i * 36));
    }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //auto elapsed = clock.getElapsedTime();
        (void) clock;
        window.clear(sf::Color(0, 100, 20));

        for (Tank* tank : tanks)
        {
            tank->draw(window);
            tank->rotation_++;
            tank->x_+=2.5f;
            tank->y_+=2.5f;
            if (tank->x_ > 1100) tank->x_ = -50;
            if (tank->y_ > 800) tank->y_ = -50;

        }
        
        window.display();
    }

    return 0;
}
