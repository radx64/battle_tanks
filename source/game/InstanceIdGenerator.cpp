#include "game/InstanceIdGenerator.hpp"

namespace game
{

uint32_t InstanceIdGenerator::getId()
{
    return nextInstanceId++;
}

uint32_t InstanceIdGenerator::nextInstanceId = 0;

}  // namespace game
