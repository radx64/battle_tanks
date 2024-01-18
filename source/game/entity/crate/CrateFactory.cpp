#include "game/entity/crate/CrateFactory.hpp"

#include <SFML/Graphics.hpp>

#include "game/entity/crate/Crate.hpp"
#include "graphics/TextureLibrary.hpp"

namespace game::entity 
{

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
    auto crate = std::make_unique<Crate>(x, y, getCrateTexture(type));
    return crate;
}

}  // namespace game::entity