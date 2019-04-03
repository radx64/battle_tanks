#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "Battle tanks DEMO!");
    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(60);


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
    sprite_body.setPosition(sf::Vector2f(150.f, 150.f));
    sf::Vector2u texture_body_size = texture_body.getSize();
    sf::Vector2f texture_body_middle_point(texture_body_size.x / 2.f, texture_body_size.y / 2.f);
    sprite_body.setOrigin(texture_body_middle_point);

    sf::Sprite sprite_cannon;
    sprite_cannon.setTexture(texture_cannon);
    sprite_cannon.setPosition(sf::Vector2f(150.f, 150.f));
    sf::Vector2u texture_cannon_size = texture_cannon.getSize();
    sf::Vector2f texture_cannon_middle_point(texture_cannon_size.x / 2.f, texture_cannon_size.y / 2.f + 10.f);
    sprite_cannon.setOrigin(texture_cannon_middle_point);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
	sprite_body.rotate(1.f);
	sprite_cannon.rotate(-1.f);
        window.draw(sprite_body);
        window.draw(sprite_cannon);
        window.display();
    }

    return 0;
}
