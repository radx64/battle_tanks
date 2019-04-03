#include <cmath>

#include <SFML/Graphics.hpp>

float to_degress(float radians)
{
    return radians * 180.0f / M_PI;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Battle tanks DEMO!");
    // Limit the framerate to 30 frames per second (this step is optional)
    window.setFramerateLimit(30);

    sf::Texture texture_body;
    if (!texture_body.loadFromFile("../sprites/tankBody_blue_outline.png"))
    {
        return -1;
    }

    sf::Texture texture_cannon;
    if (!texture_cannon.loadFromFile("../sprites/tankBlue_barrel1_outline.png"))
    {
        return -1;
    }


    sf::Sprite sprite_body;
    sprite_body.setTexture(texture_body);
    sprite_body.setPosition(sf::Vector2f(350.f, 350.f));
    sf::Vector2u texture_body_size = texture_body.getSize();
    sf::Vector2f texture_body_middle_point(texture_body_size.x / 2.f, texture_body_size.y / 2.f);
    sprite_body.setOrigin(texture_body_middle_point);

    sf::Sprite sprite_cannon;
    sprite_cannon.setTexture(texture_cannon);
    sprite_cannon.setPosition(sf::Vector2f(350.f, 350.f));
    sf::Vector2u texture_cannon_size = texture_cannon.getSize();
    sf::Vector2f texture_cannon_middle_point(texture_cannon_size.x / 2.f, texture_cannon_size.y / 2.f + 10.f);
    sprite_cannon.setOrigin(texture_cannon_middle_point);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        auto elapsed = clock.getElapsedTime();
        
        float offset_x = sin(elapsed.asMilliseconds()/500.f) *5.f;
        float offset_y = cos(elapsed.asMilliseconds()/500.f) *5.f;
        window.clear();
	    sprite_body.setRotation((to_degress(offset_x / 5.f) + to_degress(offset_y / 5.f)) + 45);
        sprite_body.move(offset_x, offset_y);
        sprite_cannon.rotate(-5.f);
        sprite_cannon.move(offset_x, offset_y);
        window.draw(sprite_body);
        window.draw(sprite_cannon);
        window.display();
    }

    return 0;
}
