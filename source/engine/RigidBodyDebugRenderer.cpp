#include "engine/RigidBodyDebugRenderer.hpp"

#include <SFML/Graphics.hpp>

#include "engine/Scene.hpp"

namespace engine
{

void RigidBodyDebugRenderer::debug(engine::Scene& scene, sf::RenderWindow& renderWindow)
{
    for (const auto& gameObject : scene.objects())
    {
        auto x = gameObject->getRigidBody().x_;
        auto y = gameObject->getRigidBody().y_;
        float radius = gameObject->getRigidBody().radius_;
        auto velocity = gameObject->getRigidBody().velocity_;

        sf::CircleShape boundary(radius, 12);
        boundary.setFillColor(sf::Color(0, 0, 0, 0));
        boundary.setOutlineThickness(1);

        if(gameObject->getRigidBody().type_ == engine::RigidBody::Type::STATIC)
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
        sf::Vertex velocityVector[] =
        {
            sf::Vertex(sf::Vector2f(x, y)),
            sf::Vertex(sf::Vector2f(x, y)+ (velocity))
        };
        renderWindow.draw(velocityVector, 2, sf::Lines);

        sf::Vertex velocityXvector[] =
        {
            sf::Vertex(sf::Vector2f(x, y), sf::Color::Red),
            sf::Vertex(sf::Vector2f(x+velocity.x, y), sf::Color::Red)
        };

        renderWindow.draw(velocityXvector, 2, sf::Lines);

        sf::Vertex velocityYvector[] =
        {
            sf::Vertex(sf::Vector2f(x, y), sf::Color::Green),
            sf::Vertex(sf::Vector2f(x, y+velocity.y), sf::Color::Green)
        };

        renderWindow.draw(velocityYvector, 2, sf::Lines);
    }
}

}  // namespace engine
