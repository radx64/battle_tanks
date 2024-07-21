#pragma once


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

