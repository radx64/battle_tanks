#ifndef GAME_WORLD_HPP_
#define GAME_WORLD_HPP_

#include <memory>

#include "game/GameObject.hpp"

namespace game
{

class World
{
public:
    std::vector<std::unique_ptr<GameObject>> objects_;
};

}  // namespace game


#endif  // GAME_WORLD_HPP_