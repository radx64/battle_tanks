#include "game/TankRenderer.hpp"

#include <cmath>

#include "math/Math.hpp"
#include "game/Tank.hpp"
#include "gui/FontLibrary.hpp"

namespace game
{
TankRenderer::TankRenderer(Tank* tank, sf::Texture& tankBody)
: tank_(tank)
{
    tankSprite_.setTexture(tankBody);
    sf::Vector2u texture_body_size = tankBody.getSize();
    auto tank_middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    tankSprite_.setOrigin(tank_middle_point);     
}

void TankRenderer::draw(sf::RenderWindow& renderWindow)
{
    auto tankRigidBody = tank_->getRigidBody();
    tankSprite_.setColor(sf::Color(10, 10, 10, 127));
    tankSprite_.setPosition(tankRigidBody.x_ + 8, tankRigidBody.y_+ 8);
    tankSprite_.setRotation(tank_->current_direction_ - 90.f);
    renderWindow.draw(tankSprite_);

    tankSprite_.setColor(sf::Color(255, 255, 255, 255));
    tankSprite_.setPosition(tankRigidBody.x_, tankRigidBody.y_);
    renderWindow.draw(tankSprite_);
    tank_->cannon_->x_ = tankRigidBody.x_;
    tank_->cannon_->y_ = tankRigidBody.y_;
    tank_->cannon_->draw(renderWindow);

    if (tank_->DEBUG_)
    {
        drawDebugInfo(renderWindow);
    }
}

void TankRenderer::drawDebugInfo(sf::RenderWindow& renderWindow)
{
    auto x = tank_->getRigidBody().x_;
    auto y = tank_->getRigidBody().y_;
    auto velocity = tank_->getRigidBody().velocity_;

    // Collision circle
    sf::CircleShape boundary(tank_->getRigidBody().radius_, 12);
    boundary.setFillColor(sf::Color(0, 0, 0, 0));
    boundary.setOutlineThickness(1);
    boundary.setOutlineColor(sf::Color(0, 0, 255));
    boundary.setOrigin(tank_->getRigidBody().radius_, tank_->getRigidBody().radius_);
    boundary.setPosition(x, y);
    renderWindow.draw(boundary);

    sf::Text debug_text;
    debug_text.setFont(gui::FontLibrary::get("armata"));
    debug_text.setPosition(x + 40, y - 20);
    debug_text.setCharacterSize(15);
    debug_text.setFillColor(sf::Color::Black);

    debug_text.setString("SPD: " + std::to_string(fabs(velocity.x + velocity.y)) + "\n" + 
        "ROT: " + std::to_string(tank_->current_direction_) + "\n" + 
        "THR: " + std::to_string(tank_->current_throttle_));
    renderWindow.draw(debug_text);

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

}  // namespace game