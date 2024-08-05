#pragma once

#include <memory>

namespace game::entity { class Bullet; }

namespace game::entity 
{

class BulletFactory
{
public:
    enum class BulletType
    {
        Dark
    };

    static std::unique_ptr<Bullet> create(const BulletFactory::BulletType type,
        float x, float y, float angle, float speed);
};

}  // namespace game::entity

