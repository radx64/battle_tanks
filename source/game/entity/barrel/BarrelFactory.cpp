#include "game/entity/barrel/BarrelFactory.hpp"

#include <SFML/Graphics.hpp>

#include "game/entity/barrel/Barrel.hpp"
#include "graphics/TextureLibrary.hpp"

namespace game::entity 
{

namespace
{
sf::Texture& getBarrelTexture(const BarrelFactory::BarrelType type)
{
    switch (type)
    {
        case BarrelFactory::BarrelType::Red    : return graphics::TextureLibrary::instance().get("barrel_red");
        case BarrelFactory::BarrelType::Rust   : return graphics::TextureLibrary::instance().get("barrel_rust");
        case BarrelFactory::BarrelType::Green  : return graphics::TextureLibrary::instance().get("barrel_green");
        case BarrelFactory::BarrelType::Black  : return graphics::TextureLibrary::instance().get("barrel_black");
        default                                : return graphics::TextureLibrary::instance().get("barrel_rust");
    }
};
}  // namespace

std::unique_ptr<Barrel> BarrelFactory::create(const BarrelFactory::BarrelType type, float x, float y)
{
    auto barrel = std::make_unique<Barrel>(x, y, getBarrelTexture(type));
    return barrel;
}

}  // namespace game::entity
