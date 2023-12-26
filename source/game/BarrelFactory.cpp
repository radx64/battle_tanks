#include "game/BarrelFactory.hpp"

#include <SFML/Graphics.hpp>

#include "game/Barrel.hpp"
#include "graphics/TextureLibrary.hpp"

namespace game 
{

// TODO create proper global object instanceId generator
uint32_t BarrelFactory::next_barrel_id = 1000;

std::unique_ptr<Barrel> BarrelFactory::create(const BarrelFactory::BarrelType type, double x, double y)
{
    auto tank = std::make_unique<Barrel>(next_barrel_id, x, y, getBarrelTexture(type));
    next_barrel_id++;
    return tank;
}

sf::Texture& BarrelFactory::getBarrelTexture(const BarrelType type)
{
    switch (type)
    {
        case BarrelType::Red    : return graphics::TextureLibrary::get("barrel_red");
        case BarrelType::Rust   : return graphics::TextureLibrary::get("barrel_rust");
        case BarrelType::Green  : return graphics::TextureLibrary::get("barrel_green");
        case BarrelType::Black  : return graphics::TextureLibrary::get("barrel_black");
        default                 : return graphics::TextureLibrary::get("barrel_rust");
    }
};

}  // namespace game