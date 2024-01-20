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
    GRASS_ROAD_LL,
    GRASS_ROAD_LR,
    GRASS_ROAD_UL,
    GRASS_ROAD_UR,
    GRASS_ROAD_CROSS,
    GRASS_ROAD_EW,
    GRASS_ROAD_NS,
    GRASS_ROAD_SPLIT_L,
    GRASS_ROAD_SPLIT_R,
    GRASS_ROAD_SPLIT_U,
    GRASS_ROAD_SPLIT_D,
    SAND_ROAD_LL,
    SAND_ROAD_LR,
    SAND_ROAD_UL,
    SAND_ROAD_UR,
    SAND_ROAD_CROSS,
    SAND_ROAD_EW,
    SAND_ROAD_NS,
    SAND_ROAD_SPLIT_L,
    SAND_ROAD_SPLIT_R,
    SAND_ROAD_SPLIT_U,
    SAND_ROAD_SPLIT_D,
    SAND1,
    SAND2,
    GRASS_SAND_NS,
    GRASS_ROAD_TRANISITON_NS,
    GRASS_ROAD_TRANISITON_SN,
    GRASS_ROAD_TRANISITON_EW,
    GRASS_ROAD_TRANISITON_WE
};

}  // namespace

namespace graphics
{

std::string toTextureName(const TileType type)
{
    switch(type)
    {
        case TileType::GRASS1 : return "grass_1";
        case TileType::GRASS2 : return "grass_2";
        case TileType::SAND1  : return "sand_1";
        case TileType::SAND2  : return "sand_2";
        case TileType::GRASS_SAND_NS : return "grass_sand_NS";
        case TileType::GRASS_ROAD_LL : return "grass_road_ll";
        case TileType::GRASS_ROAD_LR : return "grass_road_lr";
        case TileType::GRASS_ROAD_UL : return "grass_road_ul";
        case TileType::GRASS_ROAD_UR : return "grass_road_ur";
        case TileType::GRASS_ROAD_CROSS : return "grass_road_cross";
        case TileType::GRASS_ROAD_EW : return "grass_road_ew";
        case TileType::GRASS_ROAD_NS : return "grass_road_ns";
        case TileType::GRASS_ROAD_SPLIT_L : return "grass_road_split_l";
        case TileType::GRASS_ROAD_SPLIT_R : return "grass_road_split_r";
        case TileType::GRASS_ROAD_SPLIT_U : return "grass_road_split_u";
        case TileType::GRASS_ROAD_SPLIT_D : return "grass_road_split_d";
        case TileType::SAND_ROAD_LL : return "sand_road_ll";
        case TileType::SAND_ROAD_LR : return "sand_road_lr";
        case TileType::SAND_ROAD_UL : return "sand_road_ul";
        case TileType::SAND_ROAD_UR : return "sand_road_ur";
        case TileType::SAND_ROAD_CROSS : return "sand_road_cross";
        case TileType::SAND_ROAD_EW : return "sand_road_ew";
        case TileType::SAND_ROAD_NS : return "sand_road_ns";
        case TileType::SAND_ROAD_SPLIT_L : return "sand_road_split_l";
        case TileType::SAND_ROAD_SPLIT_R : return "sand_road_split_r";
        case TileType::SAND_ROAD_SPLIT_U : return "sand_road_split_u";
        case TileType::SAND_ROAD_SPLIT_D : return "sand_road_split_d";
        case TileType::GRASS_ROAD_TRANISITON_NS : return "grass_road_tranisiton_ns";
        case TileType::GRASS_ROAD_TRANISITON_SN : return "grass_road_tranisiton_sn";
        case TileType::GRASS_ROAD_TRANISITON_EW : return "grass_road_tranisiton_ew";
        case TileType::GRASS_ROAD_TRANISITON_WE : return "grass_road_tranisiton_we";
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
        case 5 : return TileType::GRASS_SAND_NS;

        case 11: return TileType::GRASS_ROAD_EW;
        case 12: return TileType::GRASS_ROAD_NS;
        case 13: return TileType::GRASS_ROAD_UL;
        case 14: return TileType::GRASS_ROAD_UR;
        case 15: return TileType::GRASS_ROAD_LR;
        case 16: return TileType::GRASS_ROAD_LL;
        case 20: return TileType::GRASS_ROAD_CROSS;
        case 21: return TileType::GRASS_ROAD_SPLIT_U;
        case 22: return TileType::GRASS_ROAD_SPLIT_R;
        case 23: return TileType::GRASS_ROAD_SPLIT_D;
        case 24: return TileType::GRASS_ROAD_SPLIT_L;

        case 25: return TileType::GRASS_ROAD_TRANISITON_NS;
        case 26: return TileType::GRASS_ROAD_TRANISITON_SN;
        case 27: return TileType::GRASS_ROAD_TRANISITON_EW;
        case 28: return TileType::GRASS_ROAD_TRANISITON_WE;

        case 31: return TileType::SAND_ROAD_EW;
        case 32: return TileType::SAND_ROAD_NS;
        case 33: return TileType::SAND_ROAD_UL;
        case 34: return TileType::SAND_ROAD_UR;
        case 35: return TileType::SAND_ROAD_LR;
        case 36: return TileType::SAND_ROAD_LL;
        case 40: return TileType::SAND_ROAD_CROSS;
        case 41: return TileType::SAND_ROAD_SPLIT_U;
        case 42: return TileType::SAND_ROAD_SPLIT_R;
        case 43: return TileType::SAND_ROAD_SPLIT_D;
        case 44: return TileType::SAND_ROAD_SPLIT_L;
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
        { 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1},
        { 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2},
        { 1,15,11,11,11,11,16, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1},
        { 2,12, 2, 1, 2, 1,12, 1, 2, 1, 2, 1, 2,15,11,11,11,11,16, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2},
        { 1,12, 1, 2, 1, 2,12, 2, 1, 2, 1, 2, 1,12, 1, 1, 2, 1,12, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1},
        { 1,12, 1, 2, 1, 2,12, 2, 1, 2, 1, 2, 1,12, 1, 1, 2, 1,12, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1},
        { 2,14,11,23,11,11,21,11,11,11,11,11,11,20,11,11,11,11,13, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2},
        { 1, 2, 1,12, 1, 2, 1, 2, 1, 2, 1, 2, 1,12, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1},
        { 2, 1, 2,12, 2, 1, 2, 1, 2, 1, 2, 1, 2,12, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2},
        { 1, 2, 1,12, 1, 2, 1, 2, 1, 2, 1, 2, 1,12, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1},
        { 5, 5, 5,25, 5, 5, 5, 5, 5, 5, 5, 5, 5,25, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        { 4, 3, 4,32, 4, 3, 4, 3, 4, 3, 4, 3, 4,32, 4, 4, 3, 4, 3, 4, 3, 4, 3, 4,35,31,31,31,36, 4},
        { 3, 4, 3,34,31,31,31,31,31,31,31,31,31,44, 3, 3, 4, 3, 4, 3, 4, 3, 4, 3,32, 3, 4, 3,32, 3},
        { 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4,32, 4, 4, 3, 4, 3, 4, 3, 4, 3, 4,32, 4, 3, 4,32, 4},
        { 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3,32, 3, 3, 4, 3, 4, 3, 4, 3, 4, 3,32, 3, 4, 3,32, 3},
        { 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4,34,31,31,31,31,31,31,31,31,31,31,44, 4, 3, 4,32, 4},
        { 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 3, 4, 3, 4, 3, 4, 3, 4, 3,34,31,31,31,33, 3},
        { 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4},
        { 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3},
        { 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 4},
    };
    sf::RenderTexture tiles_;
};

}  // namespace graphics

#endif  // GRAPHICS_TILEMAP_HPP_
