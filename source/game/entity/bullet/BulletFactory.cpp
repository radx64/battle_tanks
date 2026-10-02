#include "game/entity/bullet/BulletFactory.hpp"


#include "game/entity/bullet/Bullet.hpp"
#include "graphics/TextureLibrary.hpp"

namespace game::entity 
{

namespace
{
engine::gfx::Texture& getBulletTexture(const BulletFactory::BulletType type)
{
    switch (type)
    {
        case BulletFactory::BulletType::Dark   : return graphics::TextureLibrary::instance().get("bullet_dark");
        default                                : return graphics::TextureLibrary::instance().get("bullet_dark");
    }
};
}  // namespace

std::unique_ptr<Bullet> BulletFactory::create(const BulletFactory::BulletType type,
    float x, float y, float angle, float speed)
{
    auto bullet = std::make_unique<Bullet>(x, y, angle, speed, getBulletTexture(type));
    return bullet;
}

}  // namespace game::entity
