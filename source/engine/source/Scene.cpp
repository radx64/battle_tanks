#include "engine/Scene.hpp"

#include <algorithm>
#include <memory>

namespace engine
{

Scene::Scene()
: objects_{}
, objectsToSpawn_{}
{}

void Scene::spawnObject(std::unique_ptr<GameObject>&& newObject)
{
    objectsToSpawn_.push_back(std::move(newObject));
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

    if (not objectsToSpawn_.empty())
    {
        objects_.insert(objects_.end(), std::make_move_iterator(objectsToSpawn_.begin()), std::make_move_iterator(objectsToSpawn_.end()));
        objectsToSpawn_.clear();
    }
}

}  // namespace engine
