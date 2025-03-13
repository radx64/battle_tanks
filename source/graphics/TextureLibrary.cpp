#include "graphics/TextureLibrary.hpp"

#include "engine/Concat.hpp"
namespace
{
    constexpr char texturePath[] {"../sprites/game/"};
}

namespace graphics
{

void TextureLibrary::initTexture(const std::string& name, const std::string& path)
{
    auto placedElement = textures_.emplace(std::make_pair(name, sf::Texture()));
    bool hasLoaded = placedElement.first->second.loadFromFile(path);
    if(not hasLoaded) throw std::runtime_error("Couldn't load texture file...");
}

void TextureLibrary::initialize()
{
    using engine::concat;

    initTexture("blue_tank",            concat(texturePath, "tankBody_blue_outline.png"));
    initTexture("red_tank",             concat(texturePath, "tankBody_red_outline.png"));
    initTexture("green_tank",           concat(texturePath, "tankBody_green_outline.png"));
    initTexture("sand_tank",            concat(texturePath, "tankBody_sand_outline.png"));
    initTexture("dark_tank",            concat(texturePath, "tankBody_dark_outline.png"));
    initTexture("blue_cannon",          concat(texturePath, "tankBlue_barrel1_outline.png"));
    initTexture("red_cannon",           concat(texturePath, "tankRed_barrel1_outline.png"));
    initTexture("green_cannon",         concat(texturePath, "tankGreen_barrel1_outline.png"));
    initTexture("sand_cannon",          concat(texturePath, "tankSand_barrel1_outline.png"));
    initTexture("dark_cannon",          concat(texturePath, "tankDark_barrel1_outline.png"));
    initTexture("barrel_red",           concat(texturePath, "barrelRed_top.png"));
    initTexture("barrel_rust",          concat(texturePath, "barrelRust_top.png"));
    initTexture("barrel_green",         concat(texturePath, "barrelGreen_top.png"));
    initTexture("barrel_black",         concat(texturePath, "barrelBlack_top.png"));
    initTexture("crate_wood",           concat(texturePath, "crateWood.png"));
    initTexture("crate_metal",          concat(texturePath, "crateMetal.png"));
    initTexture("tree_green_small",     concat(texturePath, "treeGreen_small.png"));
    initTexture("tree_green_large",     concat(texturePath, "treeGreen_large.png"));
    initTexture("tree_brown_small",     concat(texturePath, "treeBrown_small.png"));
    initTexture("tree_brown_large",     concat(texturePath, "treeBrown_large.png"));
    initTexture("bullet_dark",          concat(texturePath, "bulletDark1_outline.png"));

    //      PARTICLES       //
    initTexture("explosion_smoke_1",    concat(texturePath, "explosionSmoke1.png"));
    initTexture("explosion_smoke_2",    concat(texturePath, "explosionSmoke2.png"));
    initTexture("explosion_smoke_3",    concat(texturePath, "explosionSmoke3.png"));
    initTexture("explosion_smoke_4",    concat(texturePath, "explosionSmoke4.png"));
    initTexture("explosion_smoke_5",    concat(texturePath, "explosionSmoke5.png"));

    //      ROAD TEXTURES   //
    initTexture("grass_1",                  concat(texturePath, "tileGrass1.png"));
    initTexture("grass_2",                  concat(texturePath, "tileGrass2.png"));
    initTexture("sand_1",                   concat(texturePath, "tileSand1.png"));
    initTexture("sand_2",                   concat(texturePath, "tileSand1.png"));
    initTexture("grass_sand_NS",            concat(texturePath, "tileGrass_transitionS.png"));
    initTexture("grass_sand_SN",            concat(texturePath, "tileGrass_transitionN.png"));
    initTexture("grass_road_ll",            concat(texturePath, "tileGrass_roadCornerLL.png"));
    initTexture("grass_road_lr",            concat(texturePath, "tileGrass_roadCornerLR.png"));
    initTexture("grass_road_ul",            concat(texturePath, "tileGrass_roadCornerUL.png"));
    initTexture("grass_road_ur",            concat(texturePath, "tileGrass_roadCornerUR.png"));
    initTexture("grass_road_cross",         concat(texturePath, "tileGrass_roadCrossing.png"));
    initTexture("grass_road_ew",            concat(texturePath, "tileGrass_roadEast.png"));
    initTexture("grass_road_ns",            concat(texturePath, "tileGrass_roadNorth.png"));
    initTexture("grass_road_split_l",       concat(texturePath, "tileGrass_roadSplitW.png"));
    initTexture("grass_road_split_r",       concat(texturePath, "tileGrass_roadSplitE.png"));
    initTexture("grass_road_split_u",       concat(texturePath, "tileGrass_roadSplitN.png"));
    initTexture("grass_road_split_d",       concat(texturePath, "tileGrass_roadSplitS.png"));
    initTexture("sand_road_ll",             concat(texturePath, "tileSand_roadCornerLL.png"));
    initTexture("sand_road_lr",             concat(texturePath, "tileSand_roadCornerLR.png"));
    initTexture("sand_road_ul",             concat(texturePath, "tileSand_roadCornerUL.png"));
    initTexture("sand_road_ur",             concat(texturePath, "tileSand_roadCornerUR.png"));
    initTexture("sand_road_cross",          concat(texturePath, "tileSand_roadCrossing.png"));
    initTexture("sand_road_ew",             concat(texturePath, "tileSand_roadEast.png"));
    initTexture("sand_road_ns",             concat(texturePath, "tileSand_roadNorth.png"));
    initTexture("sand_road_split_l",        concat(texturePath, "tileSand_roadSplitW.png"));
    initTexture("sand_road_split_r",        concat(texturePath, "tileSand_roadSplitE.png"));
    initTexture("sand_road_split_u",        concat(texturePath, "tileSand_roadSplitN.png"));
    initTexture("sand_road_split_d",        concat(texturePath, "tileSand_roadSplitS.png"));
    initTexture("grass_road_tranisiton_ns", concat(texturePath, "tileGrass_roadTransitionS_dirt.png"));
    initTexture("grass_road_tranisiton_sn", concat(texturePath, "tileGrass_roadTransitionN_dirt.png"));
    initTexture("grass_road_tranisiton_ew", concat(texturePath, "tileGrass_roadTransitionE_dirt.png"));
    initTexture("grass_road_tranisiton_we", concat(texturePath, "tileGrass_roadTransitionW_dirt.png"));

}

sf::Texture& TextureLibrary::get(const std::string& name)
{
    auto texture = textures_.find(name);
    if (texture == textures_.end())
        throw std::runtime_error("No texture called " + name + " found in library");

    return texture->second;
}

void TextureLibrary::destroy()
{
    textures_.clear();
}

std::map<std::string, sf::Texture> TextureLibrary::textures_;

}  // namespace graphics
