#include "game/entity/tank/TankFactory.hpp"

#include <SFML/Graphics.hpp>

#include "game/entity/tank/Cannon.hpp"
#include "game/entity/tank/Tank.hpp"
#include "graphics/TextureLibrary.hpp"

namespace game::entity 
{

namespace {
sf::Texture& getTankTexture(const TankFactory::TankType type)
{
    switch (type)
    {
        case TankFactory::TankType::Blue  : return graphics::TextureLibrary::get("blue_tank");
        case TankFactory::TankType::Red   : return graphics::TextureLibrary::get("red_tank");
        case TankFactory::TankType::Green : return graphics::TextureLibrary::get("green_tank");
        case TankFactory::TankType::Sand  : return graphics::TextureLibrary::get("sand_tank");
        case TankFactory::TankType::Black : return graphics::TextureLibrary::get("dark_tank");
        default                           : return graphics::TextureLibrary::get("blue_tank");
    }
};

sf::Texture& getCannonTexture(const TankFactory::TankType type)
{
    switch (type)
    {
        case TankFactory::TankType::Blue  : return graphics::TextureLibrary::get("blue_cannon");
        case TankFactory::TankType::Red   : return graphics::TextureLibrary::get("red_cannon");
        case TankFactory::TankType::Green : return graphics::TextureLibrary::get("green_cannon");
        case TankFactory::TankType::Sand  : return graphics::TextureLibrary::get("sand_cannon");
        case TankFactory::TankType::Black : return graphics::TextureLibrary::get("dark_cannon");
        default                           : return graphics::TextureLibrary::get("blue_cannon");
    }
};

}  // namespace

std::unique_ptr<Tank> TankFactory::create(const TankFactory::TankType type, float x, float y, float rotation, TracksRenderer* tracksRenderer)
{
    auto cannon = std::make_unique<Cannon>(x, y, rotation, getCannonTexture(type));
    auto tank = std::make_unique<Tank>(x, y, rotation, std::move(cannon), getTankTexture(type), tracksRenderer);
    return tank;
}

}  // namespace game::entity