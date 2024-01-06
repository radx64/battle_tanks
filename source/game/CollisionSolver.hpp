#ifndef GAME_COLLISION_SOLVER_HPP_
#define GAME_COLLISION_SOLVER_HPP_

namespace game {class World;}
namespace game {class RigidBody;}
namespace game {class GameObject;}

namespace game
{

class CollisionSolver
{
public:
    CollisionSolver(World& world);
    void evaluateCollisions();

protected:
    World& world_;
};

}  // namespace game

#endif  // GAME_COLLISION_SOLVER_HPP_