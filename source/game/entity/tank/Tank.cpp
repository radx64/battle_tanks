#include "game/entity/tank/Tank.hpp"

#include <algorithm>
#include <cmath>

#include "engine/Scene.hpp"
#include "engine/math/Math.hpp"
#include "engine/ParticleSystem.hpp"

#include "game/entity/tank/TankRenderer.hpp"
#include "game/entity/TracksRenderer.hpp"
#include "game/InstanceIdGenerator.hpp"
#include "graphics/TextureLibrary.hpp"

namespace game::entity 
{

constexpr float TANK_BRAKE_FORCE = 0.1;
constexpr float TANK_ACCELERATION = 6.0;
constexpr float TANK_ROTATION_SPEED = 200.0;

constexpr float TANK_RADIUS = 25;
constexpr float TANK_MASS = 5000;
constexpr float GROUND_DRAG_COEEF = 0.97;

bool Tank::DEBUG_{false};

void Tank::setDebug(bool is_enabled)
{
    DEBUG_ = is_enabled;
}

Tank::Tank(float x, float y, float rotation, 
    std::unique_ptr<Cannon> cannon, 
    sf::Texture& tank_body,
    TracksRenderer* tracks_renderer)
: current_direction_{rotation}
, cannon_{std::move(cannon)}
, led_{x , y, graphics::TextureLibrary::instance().get("led")}
, set_direction_{rotation}
, tracks_renderer_{tracks_renderer}
{

    transform().position().x = x;
    transform().position().y = y;

    renderer_ = std::make_unique<TankRenderer>(this, tank_body);

    rigid_body_ = std::make_unique<engine::RigidBody>(
        InstanceIdGenerator::getId(), 
        transform(),
        TANK_RADIUS,
        TANK_MASS, 
        GROUND_DRAG_COEEF, 
        engine::RigidBody::Type::DYNAMIC);
}

void Tank::setThrottle(float throttle)
{
    set_throttle_ = throttle;
}
void Tank::setHeading(float direction)
{
    set_direction_ = direction;
}

void Tank::setTurretHeading(float direction)
{
    cannon_->setRotation(direction);
}

void Tank::onUpdate(engine::Scene& scene, float time_step)
{
    (void) scene;

    lifetime_ += time_step;

    auto& tank_rigid_body = getRigidBody();

    // TODO Tanks now does not use rotational speed for rotation 
    // so to other collisions to work it is important to not pass fake angular velociy
    // on contacts
    tank_rigid_body.angularVelocity_ = 0;

    //Convert current direction to 0..360 range
    current_direction_ = engine::math::signedFmod(current_direction_, 360.0);

    float delta = set_direction_ - current_direction_;
    delta = engine::math::signedFmod((delta + 180.0), 360.0) - 180.0;
    // If current direction of movement is different(more than 15deg) than current one cut the throttle
    if (fabs(delta) > 15.0) current_throttle_ = 0.0; else current_throttle_ = set_throttle_;
    if (delta > 0.0) current_direction_+= std::min(TANK_ROTATION_SPEED* time_step, std::fabs(delta)) ;
    if (delta < 0.0) current_direction_-= std::min(TANK_ROTATION_SPEED* time_step, std::fabs(delta)) ;

    //TODO add some inertia calculation while accelerating
    drivetrain_force_.x = sin(engine::math::degreeToRadians(current_direction_)) * (current_throttle_ * TANK_ACCELERATION);
    drivetrain_force_.y = -cos(engine::math::degreeToRadians(current_direction_)) * (current_throttle_ * TANK_ACCELERATION);

    braking_force_.x = -tank_rigid_body.velocity_.x * TANK_BRAKE_FORCE *(1.0 - current_throttle_);
    braking_force_.y = -tank_rigid_body.velocity_.y * TANK_BRAKE_FORCE *(1.0 - current_throttle_);

    tank_rigid_body.applyForce(drivetrain_force_ + braking_force_);

    cannon_->physics(time_step);
    led_.update(time_step);

    if ((std::fabs(tank_rigid_body.velocity_.x) > 0.01) or (std::fabs(tank_rigid_body.velocity_.y) > 0.01))
    {
        auto& tankTransform = transform();

        sf::Vector2f left_track = engine::math::rotatePoint(sf::Vector2f(tankTransform.position().x-15.0, tankTransform.position().y),
            current_direction_, sf::Vector2f(tankTransform.position().x, tankTransform.position().y));
        sf::Vector2f right_track = engine::math::rotatePoint(sf::Vector2f(tankTransform.position().x+15.0, tankTransform.position().y),
            current_direction_, sf::Vector2f(tankTransform.position().x, tankTransform.position().y));

        tracks_renderer_->addTrackImprint(left_track.x, left_track.y, current_direction_);
        tracks_renderer_->addTrackImprint(right_track.x, right_track.y, current_direction_);
    }
}

void Tank::fire()
{
    cannon_->fire();
}

bool Tank::canFire() const
{
    return cannon_->canFire();
}

Cannon& Tank::cannon()
{
    return *cannon_;
}

Led& Tank::led()
{
    return led_;
}

}  // namespace game::entity
