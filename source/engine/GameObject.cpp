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

void GameObject::draw(sf::RenderWindow& renderWindow, float timeStep)
{
    (void) timeStep;
    if (renderer_)  renderer_->draw(renderWindow); // TODO add timeStep 
}

void GameObject::onUpdate(engine::Scene& scene, float timeStep)
{  
    (void) scene;
    (void) timeStep;
}

void GameObject::update(engine::Scene& scene, float timeStep)
{
    onUpdate(scene, timeStep);
    if (rigidBody_) rigidBody_->update(timeStep);
}

RigidBody& GameObject::getRigidBody()
{
    return *rigidBody_;
}

Renderer& GameObject::getRenderer()
{
    return *renderer_;
}

} // namespace engine
