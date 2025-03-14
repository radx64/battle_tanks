#include "game/particle/Explosion.hpp"

#include "graphics/TextureLibrary.hpp"

namespace game::particle
{
constexpr float EXPLOSION_LIFETIME = 1.f;

Explosion::Explosion(float x, float y)
: lifetime_{}
{ 
    position_.x = x;
    position_.y = y;
    angularVelocity_ = 180.f;
    auto& explosionBody = graphics::TextureLibrary::instance().get("explosion_smoke_2");
    sprite_.setTexture(explosionBody);
    sf::Vector2u texture_body_size = explosionBody.getSize();
    auto middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    sprite_.setOrigin(middle_point);   
}

void Explosion::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setScale(sf::Vector2f{2.f,2.f} * lifetime_);
    sprite_.setRotation(rotation_);
    sprite_.setColor(sf::Color(10, 10, 10, 127 * (EXPLOSION_LIFETIME - lifetime_)));
    sprite_.setPosition(position_ + sf::Vector2f{4.f, 4.f});
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255 * (EXPLOSION_LIFETIME - lifetime_)));
    sprite_.setPosition(position_);
    renderWindow.draw(sprite_);
}

void Explosion::onUpdate(float timeStep)
{
    lifetime_ += timeStep;

    if (lifetime_ > EXPLOSION_LIFETIME)
    {
        kill();
    }
}

}  // namespace game::particle
