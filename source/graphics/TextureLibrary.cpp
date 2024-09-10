#include "graphics/TextureLibrary.hpp"

namespace graphics
{

void TextureLibrary::initTexture(const std::string& name, const std::string& path)
{
    auto placed_element = textures_.emplace(std::make_pair(name, sf::Texture()));
    bool has_loaded = placed_element.first->second.loadFromFile(path);
    if(not has_loaded) throw std::string("Couldn't load texture file...");
}

void TextureLibrary::initialize()
{
    initTexture("blue_tank",        "../sprites/tankBody_blue_outline.png");
    initTexture("red_tank",         "../sprites/tankBody_red_outline.png");
    initTexture("green_tank",       "../sprites/tankBody_green_outline.png");
    initTexture("sand_tank",        "../sprites/tankBody_sand_outline.png");
    initTexture("dark_tank",        "../sprites/tankBody_dark_outline.png");
    initTexture("blue_cannon",      "../sprites/tankBlue_barrel1_outline.png");
    initTexture("red_cannon",       "../sprites/tankRed_barrel1_outline.png");
    initTexture("green_cannon",     "../sprites/tankGreen_barrel1_outline.png");
    initTexture("sand_cannon",      "../sprites/tankSand_barrel1_outline.png");
    initTexture("dark_cannon",      "../sprites/tankDark_barrel1_outline.png");
    initTexture("barrel_red",       "../sprites/barrelRed_top.png");
    initTexture("barrel_rust",      "../sprites/barrelRust_top.png");
    initTexture("barrel_green",     "../sprites/barrelGreen_top.png");
    initTexture("barrel_black",     "../sprites/barrelBlack_top.png");
    initTexture("crate_wood",       "../sprites/crateWood.png");
    initTexture("crate_metal",      "../sprites/crateMetal.png");
    initTexture("tree_green_small", "../sprites/treeGreen_small.png");
    initTexture("tree_green_large", "../sprites/treeGreen_large.png");
    initTexture("tree_brown_small", "../sprites/treeBrown_small.png");
    initTexture("tree_brown_large", "../sprites/treeBrown_large.png");
    initTexture("bullet_dark",      "../sprites/bulletDark1_outline.png");

    //      PARTICLES       //
    initTexture("explosion_smoke_1",    "../sprites/explosionSmoke1.png");
    initTexture("explosion_smoke_2",    "../sprites/explosionSmoke2.png");
    initTexture("explosion_smoke_3",    "../sprites/explosionSmoke3.png");
    initTexture("explosion_smoke_4",    "../sprites/explosionSmoke4.png");
    initTexture("explosion_smoke_5",    "../sprites/explosionSmoke5.png");

    //      ROAD TEXTURES   //
    initTexture("grass_1",              "../sprites/tileGrass1.png");
    initTexture("grass_2",              "../sprites/tileGrass2.png");
    initTexture("sand_1",               "../sprites/tileSand1.png");
    initTexture("sand_2",               "../sprites/tileSand1.png");
    initTexture("grass_sand_NS",        "../sprites/tileGrass_transitionS.png");
    initTexture("grass_sand_SN",        "../sprites/tileGrass_transitionN.png");
    initTexture("grass_road_ll",        "../sprites/tileGrass_roadCornerLL.png");
    initTexture("grass_road_lr",        "../sprites/tileGrass_roadCornerLR.png");
    initTexture("grass_road_ul",        "../sprites/tileGrass_roadCornerUL.png");
    initTexture("grass_road_ur",        "../sprites/tileGrass_roadCornerUR.png");
    initTexture("grass_road_cross",     "../sprites/tileGrass_roadCrossing.png");
    initTexture("grass_road_ew",        "../sprites/tileGrass_roadEast.png");
    initTexture("grass_road_ns",        "../sprites/tileGrass_roadNorth.png");
    initTexture("grass_road_split_l",   "../sprites/tileGrass_roadSplitW.png");
    initTexture("grass_road_split_r",   "../sprites/tileGrass_roadSplitE.png");
    initTexture("grass_road_split_u",   "../sprites/tileGrass_roadSplitN.png");
    initTexture("grass_road_split_d",   "../sprites/tileGrass_roadSplitS.png");
    initTexture("sand_road_ll",         "../sprites/tileSand_roadCornerLL.png");
    initTexture("sand_road_lr",         "../sprites/tileSand_roadCornerLR.png");
    initTexture("sand_road_ul",         "../sprites/tileSand_roadCornerUL.png");
    initTexture("sand_road_ur",         "../sprites/tileSand_roadCornerUR.png");
    initTexture("sand_road_cross",      "../sprites/tileSand_roadCrossing.png");
    initTexture("sand_road_ew",         "../sprites/tileSand_roadEast.png");
    initTexture("sand_road_ns",         "../sprites/tileSand_roadNorth.png");
    initTexture("sand_road_split_l",    "../sprites/tileSand_roadSplitW.png");
    initTexture("sand_road_split_r",    "../sprites/tileSand_roadSplitE.png");
    initTexture("sand_road_split_u",    "../sprites/tileSand_roadSplitN.png");
    initTexture("sand_road_split_d",    "../sprites/tileSand_roadSplitS.png");
    initTexture("grass_road_tranisiton_ns", "../sprites/tileGrass_roadTransitionS_dirt.png");
    initTexture("grass_road_tranisiton_sn", "../sprites/tileGrass_roadTransitionN_dirt.png");
    initTexture("grass_road_tranisiton_ew", "../sprites/tileGrass_roadTransitionE_dirt.png");
    initTexture("grass_road_tranisiton_we", "../sprites/tileGrass_roadTransitionW_dirt.png");

}

sf::Texture& TextureLibrary::get(const std::string& name)
{
    auto texture = textures_.find(name);
    if (texture == textures_.end())
        throw std::string("No texture called " + name + " found in library");

    return texture->second;
}

void TextureLibrary::destroy()
{
    textures_.clear();
}

std::map<std::string, sf::Texture> TextureLibrary::textures_;

}  // namespace graphics
