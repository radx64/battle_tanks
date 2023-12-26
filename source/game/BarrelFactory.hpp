#ifndef GAME_BARREL_FACTORY_HPP_
#define GAME_BARREL_FACTORY_HPP_

#include <memory>

namespace sf { class Texture; }
namespace game { class Barrel; }

namespace game 
{

class BarrelFactory
{
public:
    enum class BarrelType
    {
        Red,
        Rust,
        Green,
        Black
    };

    static std::unique_ptr<Barrel> create(const BarrelFactory::BarrelType type, double x, double y);

protected:
    static sf::Texture& getBarrelTexture(const BarrelType type);
    static uint32_t next_barrel_id;
};

}  // namespace game

#endif  // GAME_BARREL_FACTORY_HPP_