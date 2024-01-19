#ifndef ENGINE_COLLISION_SOLVER_HPP_
#define ENGINE_COLLISION_SOLVER_HPP_

namespace engine {class Scene;}
namespace engine {class RigidBody;}
namespace engine {class GameObject;}

namespace engine
{

class CollisionSolver
{
public:
    CollisionSolver(engine::Scene& scene);
    void evaluateCollisions();

protected:
    engine::Scene& scene_;
};

}  // namespace engine

#endif  // ENGINE_COLLISION_SOLVER_HPP_