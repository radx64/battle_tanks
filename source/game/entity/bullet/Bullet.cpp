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
    renderer_ = std::make_unique<BulletRenderer>(this, bulletBody);
    rigid_body_ = std::make_unique<engine::RigidBody>(
        InstanceIdGenerator::getId(), 
        x, y, BULLET_RADIUS, 
        BULLET_MASS, 
        AIR_DRAG_COEEF, 
        engine::RigidBody::Type::DYNAMIC);

    rigid_body_->rotation_ = angle;

    float angle_rad = engine::math::degree_to_radians(angle);

    float vx = speed * std::cos(angle_rad);
    float vy = speed * std::sin(angle_rad);

    rigid_body_->applyForce(sf::Vector2f{vx, vy});
}

void Bullet::onUpdate(engine::Scene& scene, float timeStep)
{
    (void) scene;
    lifetime_ += timeStep;

    if (lifetime_ > BULLET_LIFETIME)
    {
        kill();
    }
}

}  // namespace game::entity