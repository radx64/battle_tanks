#include "game/RigidBodyDebugRenderer.hpp"

#include <SFML/Graphics.hpp>

#include "game/World.hpp"

namespace game
{

void RigidBodyDebugRenderer::debug(game::World& world, sf::RenderWindow& renderWindow)
{
    for (const auto& gameObject : world.objects())
    {
        auto x = gameObject->getRigidBody().x_;
        auto y = gameObject->getRigidBody().y_;
        float radius = gameObject->getRigidBody().radius_;
        auto velocity = gameObject->getRigidBody().velocity_;

        sf::CircleShape boundary(radius, 12);
        boundary.setFillColor(sf::Color(0, 0, 0, 0));
        boundary.setOutlineThickness(1);

        if(gameObject->getRigidBody().type_ == game::RigidBody::Type::STATIC)
        {
            boundary.setOutlineColor(sf::Color(0, 0, 255));
        }
        else
        {
            boundary.setOutlineColor(sf::Color(255, 0, 0));
        }
        boundary.setOrigin(radius, radius);
        boundary.setPosition(x, y);
        renderWindow.draw(boundary);
    
        // Velocity vectors
        sf::Vertex velocity_vector[] =
        {
            sf::Vertex(sf::Vector2f(x, y)),
            sf::Vertex(sf::Vector2f(x, y)+ (velocity))
        };
        renderWindow.draw(velocity_vector, 2, sf::Lines);

        sf::Vertex velocity_x_vector[] =
        {
            sf::Vertex(sf::Vector2f(x, y), sf::Color::Red),
            sf::Vertex(sf::Vector2f(x+velocity.x, y), sf::Color::Red)
        };

        renderWindow.draw(velocity_x_vector, 2, sf::Lines);

        sf::Vertex velocity_y_vector[] =
        {
            sf::Vertex(sf::Vector2f(x, y), sf::Color::Green),
            sf::Vertex(sf::Vector2f(x, y+velocity.y), sf::Color::Green)
        };

        renderWindow.draw(velocity_y_vector, 2, sf::Lines);
    }
}

}  // namespace game
