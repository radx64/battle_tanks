#pragma once

#include "engine/GameObject.hpp"

namespace engine
{

class Scene
{
public:
    Scene();
    void spawnObject(std::unique_ptr<GameObject>&& new_object);
    const std::vector<std::unique_ptr<GameObject>>& objects();
    void update();

protected:
    std::vector<std::unique_ptr<GameObject>> objects_;
    std::vector<std::unique_ptr<GameObject>> objects_to_spawn_;
};

}  // namespace engine

