#include "game/TankRenderer.hpp"
#include "math/Math.hpp"
#include "game/Tank.hpp"

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
    tankSprite_.setColor(sf::Color(10, 10, 10, 127));
    tankSprite_.setPosition(tank_->x_ + 8, tank_->y_+ 8);
    tankSprite_.setRotation(tank_->current_direction_ - 90.f);
    renderWindow.draw(tankSprite_);

    tankSprite_.setColor(sf::Color(255, 255, 255, 255));
    tankSprite_.setPosition(tank_->x_, tank_->y_);
    renderWindow.draw(tankSprite_);
    tank_->cannon_->x_ = tank_->x_;
    tank_->cannon_->y_ = tank_->y_;
    tank_->cannon_->draw(renderWindow);

}

}  // namespace game