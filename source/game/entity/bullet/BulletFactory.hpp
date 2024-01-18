#ifndef GAME_ENTITY_BULLET_BULLET_FACTORY_HPP_
#define GAME_ENTITY_BULLET_BULLET_FACTORY_HPP_

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

#endif  // GAME_ENTITY_BULLET_BULLET_FACTORY_HPP_