#include "engine/GameObject.hpp"

#include <memory>

namespace engine {class Scene;}
namespace engine {class Renderer;}
namespace engine {class RigidBody;}

namespace sf {class RenderWindow;}

namespace engine
{

GameObject::GameObject()
: isDead_{false}
{}

bool GameObject::isDead()
{
    return isDead_;
}

void GameObject::kill()
{
    isDead_ = true;
}

void GameObject::draw(sf::RenderWindow& render_window, float time_step)
{
    (void) time_step;
    if (renderer_)  renderer_->draw(render_window); // TODO add time_step 
}

void GameObject::onUpdate(engine::Scene& scene, float time_step)
{  
    (void) scene;
    (void) time_step;
}

void GameObject::update(engine::Scene& scene, float time_step)
{
    onUpdate(scene, time_step);
    if (rigid_body_) rigid_body_->update(time_step);
}

RigidBody& GameObject::getRigidBody()
{
    return *rigid_body_;
}

Renderer& GameObject::getRenderer()
{
    return *renderer_;
}

Transform& GameObject::transform()
{
    return transform_;
}

const Transform& GameObject::transform() const
{
    return transform_;
}

} // namespace engine
