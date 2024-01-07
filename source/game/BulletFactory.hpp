#ifndef GAME_BULLET_FACTORY_HPP_
#define GAME_BULLET_FACTORY_HPP_

#include <memory>

namespace game { class Bullet; }

namespace game 
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

}  // namespace game

#endif  // GAME_BULLET_FACTORY_HPP_