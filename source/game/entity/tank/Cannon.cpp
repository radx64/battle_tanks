#include "game/entity/tank/Cannon.hpp"

#include <cmath>

#include "engine/Context.hpp"
#include "engine/math/Math.hpp"
#include "engine/ParticleSystem.hpp"
#include "engine/Scene.hpp"

#include "game/entity/bullet/Bullet.hpp"
#include "game/entity/bullet/BulletFactory.hpp"
#include "game/particle/Explosion.hpp"

namespace game::entity 
{

constexpr float CANNON_ROTATION_SPEED = 600.0;
constexpr float CANNON_COOLDOWN = 4.0f;
constexpr float CANNON_SPRITE_ROTATION_OFFSET = 90.f;
constexpr float CANNON_LENGTH = 30.f;
constexpr float BULLET_SPEED = 400.f;

Cannon::Cannon(float x, float y, float rotation, sf::Texture& texture) 
    : x_(x),
    y_(y),
    current_rotation_(rotation),
    set_rotation_(rotation),
    cooldown_{}
{
    sprite_.setTexture(texture);
    sf::Vector2u texture_size = texture.getSize();
    sf::Vector2f texture_middle_point(texture_size.x / 2, texture_size.y / 2 + 10);
    sprite_.setOrigin(texture_middle_point);
}

void Cannon::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setColor(sf::Color(10, 10, 10, 127));
    sprite_.setPosition(x_ + 3, y_+ 3);
    sprite_.setRotation(current_rotation_ + CANNON_SPRITE_ROTATION_OFFSET);
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255));
    sprite_.setPosition(x_, y_);
    renderWindow.draw(sprite_);
}

void Cannon::setRotation(float rotation)
{
    set_rotation_ = rotation;
}

void Cannon::physics(float timeStep)
{
    cooldown_ += timeStep;
    float delta = set_rotation_ - current_rotation_;
    delta = engine::math::signedFmod((delta + 180.0), 360.0) - 180.0;

    if (delta > 0.0) current_rotation_+= std::min(CANNON_ROTATION_SPEED * timeStep, std::fabs(delta));
    if (delta < 0.0) current_rotation_-= std::min(CANNON_ROTATION_SPEED * timeStep, std::fabs(delta));   
}

void Cannon::fire()
{
    if (cooldown_ > CANNON_COOLDOWN)
    {
        sf::Vector2f bullet_spawn_offset = engine::math::rotatePoint(sf::Vector2f{CANNON_LENGTH, 0.f}, current_rotation_, sf::Vector2f{0.f, 0.f});

        auto bullet_x = x_ + bullet_spawn_offset.x;
        auto bullet_y = y_ + bullet_spawn_offset.y;

        auto bullet = entity::BulletFactory::create(entity::BulletFactory::BulletType::Dark,
           bullet_x , bullet_y, current_rotation_, BULLET_SPEED);

        engine::Context::getScene().spawnObject(std::move(bullet));

        auto explosion = std::make_unique<game::particle::Explosion>(bullet_x, bullet_y);

        engine::Context::getParticleSystem().add(std::move(explosion));

        cooldown_ = 0.f;
    }
}

}  // namespace game::entity
