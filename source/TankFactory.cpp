#include "TankFactory.hpp"

#include <SFML/Graphics.hpp>

#include "Cannon.hpp"
#include "Tank.hpp"
#include "TextureLibrary.hpp"

uint32_t TankFactory::next_tank_id = 0;

std::unique_ptr<Tank> TankFactory::create_instance(const TankType type, double x, double y, double rotation)
{
    auto cannon = std::make_unique<Cannon>(x, y, rotation, get_cannon_texture(type));
    auto tank = std::make_unique<Tank>(next_tank_id, x, y, rotation, std::move(cannon), get_tank_texture(type));
    next_tank_id++;
    return tank;
}

sf::Texture& TankFactory::get_tank_texture(const TankType type)
{
    switch (type)
    {
        case TankType::Blue  : return TextureLibrary::get("blue_tank");
        case TankType::Red   : return TextureLibrary::get("red_tank");
        case TankType::Green : return TextureLibrary::get("green_tank");
        case TankType::Sand  : return TextureLibrary::get("sand_tank");
        case TankType::Black : return TextureLibrary::get("dark_tank");
        default              : return TextureLibrary::get("blue_tank");
    }
};

sf::Texture& TankFactory::get_cannon_texture(const TankType type)
{
    switch (type)
    {
        case TankType::Blue  : return TextureLibrary::get("blue_cannon");
        case TankType::Red   : return TextureLibrary::get("red_cannon");
        case TankType::Green : return TextureLibrary::get("green_cannon");
        case TankType::Sand  : return TextureLibrary::get("sand_cannon");
        case TankType::Black : return TextureLibrary::get("dark_cannon");
        default              : return TextureLibrary::get("blue_cannon");
    }
};