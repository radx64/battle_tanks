#ifndef GAME_COLLISION_SOLVER_HPP_
#define GAME_COLLISION_SOLVER_HPP_

namespace game {class Scene;}
namespace game {class RigidBody;}
namespace game {class GameObject;}

namespace game
{

class CollisionSolver
{
public:
    CollisionSolver(Scene& scene);
    void evaluateCollisions();

protected:
    Scene& scene_;
};

}  // namespace game

#endif  // GAME_COLLISION_SOLVER_HPP_