#pragma once

#include <memory>

#include "engine/Transform.hpp"
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
    void draw(sf::RenderWindow& render_window, float time_step);
    void update(engine::Scene& scene, float time_step);
    RigidBody& getRigidBody();
    Renderer& getRenderer();

    bool isDead();
    void kill();

    // This can be reimplemented for custom object behaviour every update step
    virtual void onUpdate(engine::Scene& scene, float time_step);

    Transform& transform();
    const Transform& transform() const;
    
protected:
    Transform transform_{};
    std::unique_ptr<Renderer> renderer_{};
    std::unique_ptr<RigidBody> rigid_body_{};

    bool isDead_;
};


} // namespace engine
