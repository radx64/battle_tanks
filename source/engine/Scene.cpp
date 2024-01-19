#include "engine/Scene.hpp"

#include <algorithm>
#include <memory>

namespace engine
{

Scene::Scene()
: objects_{}
, objects_to_spawn_{}
{}

void Scene::spawnObject(std::unique_ptr<GameObject>&& new_object)
{
    objects_to_spawn_.push_back(std::move(new_object));
}

const std::vector<std::unique_ptr<GameObject>>& Scene::objects()
{
    return objects_;
}

void Scene::update()
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

}  // namespace engine
