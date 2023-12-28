#include "game/CrateFactory.hpp"

#include <SFML/Graphics.hpp>

#include "game/Crate.hpp"
#include "graphics/TextureLibrary.hpp"

namespace game 
{

// TODO create proper global object instanceId generator
uint32_t CrateFactory::next_crate_id = 2000;

namespace 
{
sf::Texture& getCrateTexture(const CrateFactory::CrateType type)
{
    switch (type)
    {
        case CrateFactory::CrateType::Wood    : return graphics::TextureLibrary::get("crate_wood");
        case CrateFactory::CrateType::Metal   : return graphics::TextureLibrary::get("crate_metal");
        default                               : return graphics::TextureLibrary::get("crate_wood");
    }
};
}  // namespace

std::unique_ptr<Crate> CrateFactory::create(const CrateFactory::CrateType type, float x, float y)
{
    auto tank = std::make_unique<Crate>(next_crate_id, x, y, getCrateTexture(type));
    next_crate_id++;
    return tank;
}

}  // namespace game