#pragma once


#include <cstdint>

namespace game
{

class InstanceIdGenerator
{
public:
    static uint32_t getId();
protected:
    static uint32_t next_instance_id;
};

}  // namespace game

