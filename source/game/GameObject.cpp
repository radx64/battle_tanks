#include "game/GameObject.hpp"

#include <memory>

namespace game {class Scene;}
namespace game {class Renderer;}
namespace game {class RigidBody;}

namespace sf {class RenderWindow;}

namespace game
{

GameObject::GameObject()
: is_dead_{false}
{}

bool GameObject::isDead()
{
    return is_dead_;
}

void GameObject::kill()
{
    is_dead_ = true;
}

void GameObject::draw(sf::RenderWindow& renderWindow, float timeStep)
{
    (void) timeStep;
    if (renderer_)  renderer_->draw(renderWindow); // TODO add timeStep 
}

void GameObject::onUpdate(game::Scene& scene, float timeStep)
{  
    (void) scene;
    (void) timeStep;
}

void GameObject::update(game::Scene& scene, float timeStep)
{
    onUpdate(scene, timeStep);
    if (rigid_body_) rigid_body_->update(timeStep);
}

RigidBody& GameObject::getRigidBody()
{
    return *rigid_body_;
}

Renderer& GameObject::getRenderer()
{
    return *renderer_;
}

} // namespace game