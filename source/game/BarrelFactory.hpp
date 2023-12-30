#ifndef GAME_BARREL_FACTORY_HPP_
#define GAME_BARREL_FACTORY_HPP_

#include <memory>

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

    static std::unique_ptr<Barrel> create(const BarrelFactory::BarrelType type, float x, float y);
};

}  // namespace game

#endif  // GAME_BARREL_FACTORY_HPP_