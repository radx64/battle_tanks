#ifndef GAME_GAME_OBJECT_HPP_
#define GAME_GAME_OBJECT_HPP_

#include <memory>

#include "game/Renderer.hpp"
#include "game/RigidBody.hpp"

namespace game {class Scene;}

namespace sf {class RenderWindow;}

namespace game
{

class GameObject
{
public:
    GameObject();
    virtual ~GameObject() = default;
    void draw(sf::RenderWindow& renderWindow, float timeStep);
    void update(game::Scene& scene, float timeStep);
    RigidBody& getRigidBody();
    Renderer& getRenderer();

    bool isDead();
    void kill();

    // This can be reimplemented for custom object behaviour every update step
    virtual void onUpdate(game::Scene& scene, float timeStep);

    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<RigidBody> rigid_body_;

protected:
    bool is_dead_;
};


} // namespace game


#endif  // GAME_GAME_OBJECT_HPP_