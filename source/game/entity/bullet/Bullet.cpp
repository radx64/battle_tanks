#include "game/entity/bullet/Bullet.hpp"

#include <cmath>
#include <SFML/Graphics.hpp>

#include "engine/math/Math.hpp"
#include "game/entity/bullet/BulletRenderer.hpp"
#include "game/InstanceIdGenerator.hpp"
#include "engine/RigidBody.hpp"

namespace game::entity
{

constexpr float BULLET_RADIUS = 5.f;
constexpr float BULLET_MASS = 3.f;
constexpr float AIR_DRAG_COEEF = 0.999f;

constexpr float BULLET_LIFETIME = 7.5f;

Bullet::Bullet(float x, float y, float angle, float speed, sf::Texture& bulletBody)
: lifetime_{}
{ 
    transform().position().x = x;
    transform().position().y = y;
    renderer_ = std::make_unique<BulletRenderer>(this, bulletBody);
    rigid_body_ = std::make_unique<engine::RigidBody>(
        InstanceIdGenerator::getId(), 
        transform(),
        BULLET_RADIUS, 
        BULLET_MASS, 
        AIR_DRAG_COEEF, 
        engine::RigidBody::Type::DYNAMIC);

    transform().rotation() = angle;

    float angle_rad = engine::math::degreeToRadians(angle);

    // This is game convention coordinate system Y+ downwards 0deg up
    // so this need to reverse math convention (where right (X+) is 0 deg)
    // that why vx and vy are not cos and sin of rad but other way around
    float vx = speed * std::sin(angle_rad);
    float vy = speed * -std::cos(angle_rad);

    rigid_body_->applyForce(sf::Vector2f{vx, vy});
}

void Bullet::onUpdate(engine::Scene& scene, float time_step)
{
    (void) scene;
    lifetime_ += time_step;

    if (lifetime_ > BULLET_LIFETIME)
    {
        kill();
    }
}

}  // namespace game::entity
