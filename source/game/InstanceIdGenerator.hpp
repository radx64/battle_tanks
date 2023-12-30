#ifndef GAME_INSTANCE_ID_GENERATOR_HPP_
#define GAME_INSTANCE_ID_GENERATOR_HPP_

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

#endif  // GAME_INSTANCE_ID_GENERATOR_HPP_