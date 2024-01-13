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
    initTexture("grass_1",          "../sprites/tileGrass1.png");
    initTexture("grass_2",          "../sprites/tileGrass2.png");
    initTexture("sand_1",           "../sprites/tileSand1.png");
    initTexture("sand_2",           "../sprites/tileSand1.png");
    initTexture("grass_sand_NS",    "../sprites/tileGrass_transitionS.png");
    initTexture("grass_sand_SN",    "../sprites/tileGrass_transitionN.png");
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
    initTexture("explosion_smoke_1","../sprites/explosionSmoke1.png");
    initTexture("explosion_smoke_2","../sprites/explosionSmoke2.png");
    initTexture("explosion_smoke_3","../sprites/explosionSmoke3.png");
    initTexture("explosion_smoke_4","../sprites/explosionSmoke4.png");
    initTexture("explosion_smoke_5","../sprites/explosionSmoke5.png");
}

sf::Texture& TextureLibrary::get(const std::string& name)
{
    auto texture = textures_.find(name);
    if (texture == textures_.end())
        throw std::string("No texture called " + name + " found in library");

    return texture->second;
}

std::map<std::string, sf::Texture> TextureLibrary::textures_;

}  // namespace graphics
