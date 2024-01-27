#include "game/entity/tank/Tank.hpp"

#include <algorithm>
#include <cmath>

#include "engine/math/Math.hpp"
#include "engine/ParticleSystem.hpp"
#include "engine/Scene.hpp"
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
    sf::Texture& tankBody,
    TracksRenderer* tracksRenderer)
: current_direction_(rotation)
, cannon_(std::move(cannon))
, set_direction_(rotation)
, tracks_renderer_(tracksRenderer)
{
    renderer_ = std::make_unique<TankRenderer>(this, tankBody);

    rigid_body_ = std::make_unique<engine::RigidBody>(
        InstanceIdGenerator::getId(), 
        x, y, TANK_RADIUS,
        TANK_MASS, 
        GROUND_DRAG_COEEF, 
        engine::RigidBody::Type::DYNAMIC);
}

void Tank::setThrottle(float throttle)
{
    set_throttle_ = throttle;
}
void Tank::setDirection(float direction)
{
    set_direction_ = direction;
    cannon_->setRotation(direction);
}

void Tank::onUpdate(engine::Scene& scene, float timeStep)
{
    (void) scene;

    lifetime_ += timeStep;

    auto& tank_rigid_body = getRigidBody();

    // TODO Tanks now does not use rotational speed for rotation 
    // so to other collisions to work it is important to not pass fake angular velociy
    // on contacts
    tank_rigid_body.angular_velocity_ = 0;

    //Convert current direction to 0..360 range
    current_direction_ = engine::math::signed_fmod(current_direction_, 360.0);

    float delta = set_direction_ - current_direction_;
    delta = engine::math::signed_fmod((delta + 180.0), 360.0) - 180.0;
    // If current direction of movement is different(more than 15deg) than current one cut the throttle
    if (fabs(delta) > 15.0) current_throttle_ = 0.0; else current_throttle_ = set_throttle_;
    if (delta > 0.0) current_direction_+= std::min(TANK_ROTATION_SPEED* timeStep, std::fabs(delta)) ;
    if (delta < 0.0) current_direction_-= std::min(TANK_ROTATION_SPEED* timeStep, std::fabs(delta)) ;

    //TODO add some inertia calculation while accelerating
    drivetrain_force_.x = cos(current_direction_ * M_PI/180.0) * (current_throttle_ * TANK_ACCELERATION);
    drivetrain_force_.y = sin(current_direction_ * M_PI/180.0) * (current_throttle_ * TANK_ACCELERATION);

    braking_force_.x = -tank_rigid_body.velocity_.x * TANK_BRAKE_FORCE *(1.0 - current_throttle_);
    braking_force_.y = -tank_rigid_body.velocity_.y * TANK_BRAKE_FORCE *(1.0 - current_throttle_);

    tank_rigid_body.applyForce(drivetrain_force_ + braking_force_);

    cannon_->physics(timeStep);

    if ((std::fabs(tank_rigid_body.velocity_.x) > 0.01) or (std::fabs(tank_rigid_body.velocity_.y) > 0.01))
    {
        sf::Vector2f left_track = engine::math::rotate_point(sf::Vector2f(tank_rigid_body.x_, tank_rigid_body.y_-15.0),
            current_direction_, sf::Vector2f(tank_rigid_body.x_, tank_rigid_body.y_));
        sf::Vector2f right_track = engine::math::rotate_point(sf::Vector2f(tank_rigid_body.x_, tank_rigid_body.y_+15.0),
            current_direction_, sf::Vector2f(tank_rigid_body.x_, tank_rigid_body.y_));

        tracks_renderer_->addTrackImprint(left_track.x, left_track.y, current_direction_);
        tracks_renderer_->addTrackImprint(right_track.x, right_track.y, current_direction_);
    }

    // Testing bullet shooting
    // Cannon has some reload timer so trying to fire every update
    // is fine 🤣
    cannon_->fire();
}

}  // namespace game::entity
