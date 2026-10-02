#pragma once

namespace engine { class Scene; }
namespace engine::gfx { class RenderTarget; }

namespace engine
{

class RigidBodyDebugRenderer
{
public:
    static void debug(engine::Scene& scene, engine::gfx::RenderTarget& renderTarget);
};

}  // namespace engine
