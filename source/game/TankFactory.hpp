#ifndef GAME_TANK_FACTORY_HPP_
#define GAME_TANK_FACTORY_HPP_

#include <memory>

namespace sf { class Texture; }
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

protected:
    static sf::Texture& getTankTexture(const TankType type);
    static sf::Texture& getCannonTexture(const TankType type);
    static uint32_t next_tank_id;
};

}  // namespace game

#endif  // GAME_TANK_FACTORY_HPP_