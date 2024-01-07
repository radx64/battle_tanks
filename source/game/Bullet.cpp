#include "game/Bullet.hpp"

#include <cmath>
#include <SFML/Graphics.hpp>

#include "math/Math.hpp"
#include "game/InstanceIdGenerator.hpp"
#include "game/BulletRenderer.hpp"
#include "game/RigidBody.hpp"

namespace game
{

constexpr float BULLET_RADIUS = 5;
constexpr float BULLET_MASS = 20;
constexpr float AIR_DRAG_COEEF = 0.999;

Bullet::Bullet(float x, float y, float angle, float speed, sf::Texture& bulletBody)
{ 
    renderer_ = std::make_unique<BulletRenderer>(this, bulletBody);
    rigid_body_ = std::make_unique<RigidBody>(
        InstanceIdGenerator::getId(), 
        x, y, BULLET_RADIUS, 
        BULLET_MASS, 
        AIR_DRAG_COEEF, 
        RigidBody::Type::DYNAMIC);

    rigid_body_->rotation_ = angle;

    float angle_rad = math::degree_to_radians(angle);

    float vx = speed * std::cos(angle_rad);
    float vy = speed * std::sin(angle_rad);

    rigid_body_->applyForce(sf::Vector2f{vx, vy});
}

}  // namespace game