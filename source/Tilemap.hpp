#include <cstdint>
#include <string>

#include <SFML/Graphics.hpp>

#include "TextureLibrary.hpp"

enum class TileType
{
    GRASS1,
    GRASS2,
    SAND1,
    SAND2,
    GRASS_SAND_N
};

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
    void draw(sf::RenderWindow& renderWindow)
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
        for (int x = 0; x < 15; ++x)
        {
            for(int y = 0; y < 10; ++y)
            {
                tiles_.emplace_back(Tile(x * 64, y*64, idToType(tileIds_[y][x])));
            }
        }
    }
    void draw(sf::RenderWindow& renderWindow)
    {
        for (auto tile : tiles_)
        {
            tile.draw(renderWindow);
        }
    }

private:
    uint8_t tileIds_[10][15] = 
    {
        {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1},
        {2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
        {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1},
        {2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
        {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1},
        {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {3,4,3,4,3,4,3,4,3,4,3,4,3,4,3},
        {4,3,4,3,4,3,4,3,4,3,4,3,4,3,4},
        {3,4,3,4,3,4,3,4,3,4,3,4,3,4,3},
        {4,3,4,3,4,3,4,3,4,3,4,3,4,3,4},
    };
    std::vector<Tile> tiles_;
};