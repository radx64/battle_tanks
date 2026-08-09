#include "engine/RigidBodyDebugRenderer.hpp"

#include <SFML/Graphics.hpp>

#include "engine/Scene.hpp"

namespace engine
{

void RigidBodyDebugRenderer::debug(engine::Scene& scene, sf::RenderWindow& render_window)
{
    for (const auto& gameObject : scene.objects())
    {
        auto x = gameObject->getRigidBody().transform().position().x;
        auto y = gameObject->getRigidBody().transform().position().y;
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
       render_window.draw(boundary);
    
        // Velocity vectors
        sf::Vertex velocityVector[] =
        {
            sf::Vertex(sf::Vector2f(x, y)),
            sf::Vertex(sf::Vector2f(x, y)+ (velocity))
        };
       render_window.draw(velocityVector, 2, sf::Lines);

        sf::Vertex velocityXvector[] =
        {
            sf::Vertex(sf::Vector2f(x, y), sf::Color::Red),
            sf::Vertex(sf::Vector2f(x+velocity.x, y), sf::Color::Red)
        };

       render_window.draw(velocityXvector, 2, sf::Lines);

        sf::Vertex velocityYvector[] =
        {
            sf::Vertex(sf::Vector2f(x, y), sf::Color::Green),
            sf::Vertex(sf::Vector2f(x, y+velocity.y), sf::Color::Green)
        };

       render_window.draw(velocityYvector, 2, sf::Lines);
    }
}

}  // namespace engine
