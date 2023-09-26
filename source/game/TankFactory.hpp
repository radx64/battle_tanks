#ifndef GAME_TANK_FACTORY_HPP_
#define GAME_TANK_FACTORY_HPP_

#include <memory>

namespace sf { class Texture;}
class Tank;

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

    static std::unique_ptr<Tank> create_instance(const TankType type, double x, double y, double rotation);

protected:
    static sf::Texture& get_tank_texture(const TankType type);
    static sf::Texture& get_cannon_texture(const TankType type);
    static uint32_t next_tank_id;
};

#endif  // GAME_TANK_FACTORY_HPP_