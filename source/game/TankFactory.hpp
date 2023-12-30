#ifndef GAME_TANK_FACTORY_HPP_
#define GAME_TANK_FACTORY_HPP_

#include <memory>

namespace game { class Tank; }

namespace game 
{

class TankFactory
{
public:
    enum class TankType
    {
        Red,
        Green,
        Blue,
        Sand,
        Black
    };

    static std::unique_ptr<Tank> create(const TankFactory::TankType type, float x, float y, float rotation);
};

}  // namespace game

#endif  // GAME_TANK_FACTORY_HPP_