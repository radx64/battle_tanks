#include "WaitCondition.hpp"

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
   
}  // namespace game::lua
