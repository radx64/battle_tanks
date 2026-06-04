#include "game/lua/WaitCondition.hpp"

#include "game/entity/tank/Tank.hpp"

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
   
}  // namespace game::lua
