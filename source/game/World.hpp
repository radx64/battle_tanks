#ifndef GAME_WORLD_HPP_
#define GAME_WORLD_HPP_

#include <algorithm>
#include <memory>

#include "game/GameObject.hpp"

namespace game
{

class World
{
public:
    World()
    : objects_{}
    , objects_to_spawn_{}
    {}

    void spawnObject(std::unique_ptr<GameObject>&& new_object)
    {
        objects_to_spawn_.push_back(std::move(new_object));
    }

    const std::vector<std::unique_ptr<GameObject>>& objects()
    {
        return objects_;
    }

    void update()
    {
        objects_.erase(
            std::remove_if(objects_.begin(), objects_.end(),
                [](auto& object){ return object->isDead();}),
            objects_.end());

        if (not objects_to_spawn_.empty())
        {
            objects_.insert(objects_.end(), std::make_move_iterator(objects_to_spawn_.begin()), std::make_move_iterator(objects_to_spawn_.end()));
            objects_to_spawn_.clear();
        }
    }

protected:
    std::vector<std::unique_ptr<GameObject>> objects_;
    std::vector<std::unique_ptr<GameObject>> objects_to_spawn_;
};

}  // namespace game


#endif  // GAME_WORLD_HPP_