#include "game/InstanceIdGenerator.hpp"

namespace game
{

uint32_t InstanceIdGenerator::getId()
{
    return next_instance_id++;
}

uint32_t InstanceIdGenerator::next_instance_id = 0;

}  // namespace game