#ifndef GRAPHICS_TILEMAP_HPP_
#define GRAPHICS_TILEMAP_HPP_

#include <cstdint>
#include <string>

#include <SFML/Graphics.hpp>

#include "graphics/TextureLibrary.hpp"

namespace {

constexpr auto WIDTH = 30;
constexpr auto HEIGHT = 20;

enum class TileType
{
    GRASS1,
    GRASS2,
    SAND1,
    SAND2,
    GRASS_SAND_N
};

}  // namespace

std::string toTextureName(const TileType type)
{
    switch(type)
    {
        case TileType::GRASS1 : return "grass_1";
        case TileType::GRASS2 : return "grass_2";
        case TileType::SAND1  : return "sand_1";
        case TileType::SAND2  : return "sand_2";
        case TileType::GRASS_SAND_N : return "grass_sand_NS";
        default : return "unknown";
    }
}

class Tile
{
public:
    Tile(const float x, const float y, const TileType type)
    {  
        sprite_.setTexture(TextureLibrary::get(toTextureName(type)));
        sprite_.setPosition(x, y);
    }

    //TODO: implement culling
    void draw(sf::RenderTexture& renderWindow)
    {
        renderWindow.draw(sprite_);
    }
private:
    sf::Sprite sprite_;
};

TileType idToType(const uint8_t id)
{
    switch(id)
    {
        case 1 : return TileType::GRASS1;
        case 2 : return TileType::GRASS2;
        case 3 : return TileType::SAND1; 
        case 4 : return TileType::SAND2; 
        case 5 : return TileType::GRASS_SAND_N;
    }
    return TileType::GRASS1;
}

class Tilemap
{
public:
    Tilemap()
    {
        tiles_.create(WIDTH * 64, HEIGHT * 64);
        for (int x = 0; x < WIDTH; ++x)
        {
            for(int y = 0; y < HEIGHT; ++y)
            {
                auto tile = Tile(x * 64, y*64, idToType(tileIds_[y][x]));
                tile.draw(tiles_);
            }
        }
        tiles_.display();
    }
    void draw(sf::RenderWindow& renderWindow)
    {
        sf::Sprite texture(tiles_.getTexture());
        renderWindow.draw(texture);
    }

private:
    uint8_t tileIds_[HEIGHT][WIDTH] = 
    {
        {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1},
        {2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
        {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1},
        {2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
        {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1},
        {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1},
        {2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
        {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1},
        {2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
        {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1},
        {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4},
        {3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3},
        {4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4},
        {3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3},
        {4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4},
        {3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3},
        {4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4},
        {3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3},
        {4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4},
    };
    sf::RenderTexture tiles_;
};

#endif  // GRAPHICS_TILEMAP_HPP_
