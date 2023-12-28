#include "game/Tree.hpp"

#include <SFML/Graphics.hpp>

#include "graphics/TextureLibrary.hpp"

namespace game
{

constexpr float SMALL_TREE_TRUNK_RADIUS = 8.f;
constexpr float LARGE_TREE_TRUNK_RADIUS = 16.f;

constexpr float GROUND_DRAG_COEEF = 0.1f;
constexpr float TREE_MASS = 999999.f;

namespace {
sf::Texture& getTreeTexture(const Tree::Type type)
{
    switch (type)
    {
        case Tree::Type::Brown_Large    : return graphics::TextureLibrary::get("tree_brown_large");
        case Tree::Type::Brown_Small    : return graphics::TextureLibrary::get("tree_brown_small"); 
        case Tree::Type::Green_Large    : return graphics::TextureLibrary::get("tree_green_large");
        case Tree::Type::Green_Small    : return graphics::TextureLibrary::get("tree_green_small");
    };
}

float getTreeTrunkRadius(const Tree::Type type)
{
    switch (type)
    {
        case Tree::Type::Brown_Large    : return LARGE_TREE_TRUNK_RADIUS;
        case Tree::Type::Brown_Small    : return SMALL_TREE_TRUNK_RADIUS;
        case Tree::Type::Green_Large    : return LARGE_TREE_TRUNK_RADIUS;
        case Tree::Type::Green_Small    : return SMALL_TREE_TRUNK_RADIUS;
    };
}

} // namespace

Tree::Tree(uint32_t id, float x, float y, const Type& treeType)
: RigidBody(id, x, y, getTreeTrunkRadius(treeType), TREE_MASS, GROUND_DRAG_COEEF)
{
    auto& texture = getTreeTexture(treeType);
    ///graphics::TextureLibrary::get("tree_green_small");
    sprite_.setTexture(texture);
    sf::Vector2u texture_body_size = texture.getSize();
    auto middle_point = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    sprite_.setOrigin(middle_point);
}

void Tree::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setRotation(angle_);
    sprite_.setColor(sf::Color(10, 10, 10, 127));
    sprite_.setPosition(x_ + 16, y_+ 16);
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255));
    sprite_.setPosition(x_, y_);
    renderWindow.draw(sprite_);
}

}  // namespace game