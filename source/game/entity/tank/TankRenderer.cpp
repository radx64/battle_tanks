#include "game/entity/tank/TankRenderer.hpp"

#include <cmath>

#include "math/Math.hpp"
#include "game/entity/tank/Tank.hpp"
#include "gui/FontLibrary.hpp"

namespace game::entity
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

    sf::Text debug_text;
    debug_text.setFont(gui::FontLibrary::get("armata"));
    debug_text.setPosition(x + 40, y - 20);
    debug_text.setCharacterSize(15);
    debug_text.setFillColor(sf::Color::Black);

    debug_text.setString("SPD: " + std::to_string(fabs(velocity.x + velocity.y)) + "\n" + 
        "ROT: " + std::to_string(tank_->current_direction_) + "\n" + 
        "THR: " + std::to_string(tank_->current_throttle_));
    renderWindow.draw(debug_text);
}

}  // namespace game::entity