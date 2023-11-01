#include "game/TankFactory.hpp"

#include <SFML/Graphics.hpp>

#include "game/Cannon.hpp"
#include "game/Tank.hpp"
#include "graphics/TextureLibrary.hpp"

namespace game 
{

uint32_t TankFactory::next_tank_id = 0;

std::unique_ptr<Tank> TankFactory::createInstance(const TankFactory::TankType type, double x, double y, double rotation)
{
    auto cannon = std::make_unique<Cannon>(x, y, rotation, getCannonTexture(type));
    auto tank = std::make_unique<Tank>(next_tank_id, x, y, rotation, std::move(cannon), getTankTexture(type));
    next_tank_id++;
    return tank;
}

sf::Texture& TankFactory::getTankTexture(const TankType type)
{
    switch (type)
    {
        case TankType::Blue  : return graphics::TextureLibrary::get("blue_tank");
        case TankType::Red   : return graphics::TextureLibrary::get("red_tank");
        case TankType::Green : return graphics::TextureLibrary::get("green_tank");
        case TankType::Sand  : return graphics::TextureLibrary::get("sand_tank");
        case TankType::Black : return graphics::TextureLibrary::get("dark_tank");
        default              : return graphics::TextureLibrary::get("blue_tank");
    }
};

sf::Texture& TankFactory::getCannonTexture(const TankType type)
{
    switch (type)
    {
        case TankType::Blue  : return graphics::TextureLibrary::get("blue_cannon");
        case TankType::Red   : return graphics::TextureLibrary::get("red_cannon");
        case TankType::Green : return graphics::TextureLibrary::get("green_cannon");
        case TankType::Sand  : return graphics::TextureLibrary::get("sand_cannon");
        case TankType::Black : return graphics::TextureLibrary::get("dark_cannon");
        default              : return graphics::TextureLibrary::get("blue_cannon");
    }
};

}  // namespace game