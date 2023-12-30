#include "game/TreeFactory.hpp"

#include <SFML/Graphics.hpp>

#include "game/Tree.hpp"
#include "graphics/TextureLibrary.hpp"

namespace game 
{

constexpr float SMALL_TREE_TRUNK_RADIUS = 8.f;
constexpr float LARGE_TREE_TRUNK_RADIUS = 16.f;

namespace {
sf::Texture& getTreeTexture(const TreeFactory::TreeType type)
{
    switch (type)
    {
        case TreeFactory::TreeType::Brown_Large    : return graphics::TextureLibrary::get("tree_brown_large");
        case TreeFactory::TreeType::Brown_Small    : return graphics::TextureLibrary::get("tree_brown_small"); 
        case TreeFactory::TreeType::Green_Large    : return graphics::TextureLibrary::get("tree_green_large");
        case TreeFactory::TreeType::Green_Small    : return graphics::TextureLibrary::get("tree_green_small");
        default                                    : return graphics::TextureLibrary::get("tree_green_small");
    };
}

float getTreeTrunkRadius(const TreeFactory::TreeType type)
{
    switch (type)
    {
        case TreeFactory::TreeType::Brown_Large    : return LARGE_TREE_TRUNK_RADIUS;
        case TreeFactory::TreeType::Brown_Small    : return SMALL_TREE_TRUNK_RADIUS;
        case TreeFactory::TreeType::Green_Large    : return LARGE_TREE_TRUNK_RADIUS;
        case TreeFactory::TreeType::Green_Small    : return SMALL_TREE_TRUNK_RADIUS;
        default                                    : return SMALL_TREE_TRUNK_RADIUS;
    };
}

} // namespace

std::unique_ptr<Tree> TreeFactory::create(const TreeFactory::TreeType type, float x, float y)
{
    auto tank = std::make_unique<Tree>(x, y, getTreeTexture(type), getTreeTrunkRadius(type));
    return tank;
}
}  // namespace game