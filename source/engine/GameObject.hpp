#pragma once

#include <memory>

#include "engine/Renderer.hpp"
#include "engine/RigidBody.hpp"

namespace engine {class Scene;}

namespace sf {class RenderWindow;}

namespace engine
{

class GameObject
{
public:
    GameObject();
    virtual ~GameObject() = default;
    void draw(sf::RenderWindow& renderWindow, float timeStep);
    void update(engine::Scene& scene, float timeStep);
    RigidBody& getRigidBody();
    Renderer& getRenderer();

    bool isDead();
    void kill();

    // This can be reimplemented for custom object behaviour every update step
    virtual void onUpdate(engine::Scene& scene, float timeStep);

    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<RigidBody> rigidBody_;

protected:
    bool isDead_;
};


} // namespace engine
