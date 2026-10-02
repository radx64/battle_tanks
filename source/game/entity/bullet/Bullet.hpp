#pragma once


#include "engine/GameObject.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity
{

class Bullet : public engine::GameObject
{
public:
    Bullet(float x, float y, float angle, float speed, engine::gfx::Texture& bulletBody);
    virtual ~Bullet() = default;

protected:
    void onUpdate(engine::Scene& scene, float time_step) override;

    float lifetime_;
};

}  // namespace game::entity
