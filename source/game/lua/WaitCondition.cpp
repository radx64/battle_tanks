#include "game/lua/WaitCondition.hpp"

#include <algorithm>
#include <cmath>

#include "game/entity/tank/Tank.hpp"
#include "engine/math/Math.hpp"

namespace game::lua
{

WaitSeconds::WaitSeconds(float seconds)
: remaining_{seconds}
{}

bool WaitSeconds::update(float dt)
{
    remaining_ -= dt;
    return remaining_ <= 0.0f;
}

WaitFireCycle::WaitFireCycle(game::entity::Tank* tank)
: tank_(tank)
{}

bool WaitFireCycle::update(float)
{
    if (!tank_) {
        return true;
    }

    if (!tank_->canFire()) {
        return false;
    }

    tank_->fire();
    return true;
}

// TODO: this is a crude throttle falloff implementation, 
// improve later, maybe add some PID controller or something
static float compute_move_to_throttle(float distance)
{
    constexpr float full_speed_distance = 50.0f;
    float t = std::max(0.4f, distance / full_speed_distance);
    return std::min(1.0f, t * t);
}

WaitMoveTo::WaitMoveTo(game::entity::Tank* tank, float targetX, float targetY, float threshold)
: tank_{tank}
, target_x_{targetX}
, target_y_{targetY}
, threshold_{threshold}
{
    if (!tank_) return;

    const auto& rb = tank_->transform();
    float dx = target_x_ - rb.position().x;
    float dy = target_y_ - rb.position().y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance <= threshold_) {
        tank_->setThrottle(0.0f);
        return;
    }

    float direction = std::atan2(dx, -dy);
    float heading = engine::math::radiansToDegree(direction);
    float throttle = compute_move_to_throttle(distance);

    tank_->setHeading(heading);
    tank_->setTurretHeading(heading);
    tank_->setThrottle(throttle);
}

bool WaitMoveTo::update(float)
{
    if (!tank_) return true;

    const auto& rb = tank_->transform();
    float dx = target_x_ - rb.position().x;
    float dy = target_y_ - rb.position().y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance <= threshold_) {
        tank_->setThrottle(0.0f);
        return true;
    }

    float direction = std::atan2(dx, -dy);
    float heading = engine::math::radiansToDegree(direction);
    float throttle = compute_move_to_throttle(distance);

    tank_->setHeading(heading);
    tank_->setTurretHeading(heading);
    tank_->setThrottle(throttle);

    return false;
}
   
}  // namespace game::lua
