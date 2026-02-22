#pragma once

#include "engine/Concat.hpp"
#include "engine/TextureLibrary.hpp"

namespace
{
    constexpr char texturePath[] {"./resources/sprites/game/"};
}

namespace graphics
{
class GraphicsAssetsList
{
public:
    static const std::map<std::string, std::string>& getAssets()
    {
        using engine::concat;
        static const std::map<std::string, std::string> assets
        {

            {"blue_tank",            concat(texturePath, "tankBody_blue_outline.png")},
            {"red_tank",             concat(texturePath, "tankBody_red_outline.png")},
            {"green_tank",           concat(texturePath, "tankBody_green_outline.png")},
            {"sand_tank",            concat(texturePath, "tankBody_sand_outline.png")},
            {"dark_tank",            concat(texturePath, "tankBody_dark_outline.png")},
            {"blue_cannon",          concat(texturePath, "tankBlue_barrel1_outline.png")},
            {"red_cannon",           concat(texturePath, "tankRed_barrel1_outline.png")},
            {"green_cannon",         concat(texturePath, "tankGreen_barrel1_outline.png")},
            {"sand_cannon",          concat(texturePath, "tankSand_barrel1_outline.png")},
            {"dark_cannon",          concat(texturePath, "tankDark_barrel1_outline.png")},
            {"barrel_red",           concat(texturePath, "barrelRed_top.png")},
            {"barrel_rust",          concat(texturePath, "barrelRust_top.png")},
            {"barrel_green",         concat(texturePath, "barrelGreen_top.png")},
            {"barrel_black",         concat(texturePath, "barrelBlack_top.png")},
            {"crate_wood",           concat(texturePath, "crateWood.png")},
            {"crate_metal",          concat(texturePath, "crateMetal.png")},
            {"tree_green_small",     concat(texturePath, "treeGreen_small.png")},
            {"tree_green_large",     concat(texturePath, "treeGreen_large.png")},
            {"tree_brown_small",     concat(texturePath, "treeBrown_small.png")},
            {"tree_brown_large",     concat(texturePath, "treeBrown_large.png")},
            {"bullet_dark",          concat(texturePath, "bulletDark1_outline.png")},
        
            //      PARTICLES       //
            {"explosion_smoke_1",    concat(texturePath, "explosionSmoke1.png")},
            {"explosion_smoke_2",    concat(texturePath, "explosionSmoke2.png")},
            {"explosion_smoke_3",    concat(texturePath, "explosionSmoke3.png")},
            {"explosion_smoke_4",    concat(texturePath, "explosionSmoke4.png")},
            {"explosion_smoke_5",    concat(texturePath, "explosionSmoke5.png")},
        
            //      ROAD TEXTURES   //
            {"grass_1",                  concat(texturePath, "tileGrass1.png")},
            {"grass_2",                  concat(texturePath, "tileGrass2.png")},
            {"sand_1",                   concat(texturePath, "tileSand1.png")},
            {"sand_2",                   concat(texturePath, "tileSand1.png")},
            {"grass_sand_NS",            concat(texturePath, "tileGrass_transitionS.png")},
            {"grass_sand_SN",            concat(texturePath, "tileGrass_transitionN.png")},
            {"grass_road_ll",            concat(texturePath, "tileGrass_roadCornerLL.png")},
            {"grass_road_lr",            concat(texturePath, "tileGrass_roadCornerLR.png")},
            {"grass_road_ul",            concat(texturePath, "tileGrass_roadCornerUL.png")},
            {"grass_road_ur",            concat(texturePath, "tileGrass_roadCornerUR.png")},
            {"grass_road_cross",         concat(texturePath, "tileGrass_roadCrossing.png")},
            {"grass_road_ew",            concat(texturePath, "tileGrass_roadEast.png")},
            {"grass_road_ns",            concat(texturePath, "tileGrass_roadNorth.png")},
            {"grass_road_split_l",       concat(texturePath, "tileGrass_roadSplitW.png")},
            {"grass_road_split_r",       concat(texturePath, "tileGrass_roadSplitE.png")},
            {"grass_road_split_u",       concat(texturePath, "tileGrass_roadSplitN.png")},
            {"grass_road_split_d",       concat(texturePath, "tileGrass_roadSplitS.png")},
            {"sand_road_ll",             concat(texturePath, "tileSand_roadCornerLL.png")},
            {"sand_road_lr",             concat(texturePath, "tileSand_roadCornerLR.png")},
            {"sand_road_ul",             concat(texturePath, "tileSand_roadCornerUL.png")},
            {"sand_road_ur",             concat(texturePath, "tileSand_roadCornerUR.png")},
            {"sand_road_cross",          concat(texturePath, "tileSand_roadCrossing.png")},
            {"sand_road_ew",             concat(texturePath, "tileSand_roadEast.png")},
            {"sand_road_ns",             concat(texturePath, "tileSand_roadNorth.png")},
            {"sand_road_split_l",        concat(texturePath, "tileSand_roadSplitW.png")},
            {"sand_road_split_r",        concat(texturePath, "tileSand_roadSplitE.png")},
            {"sand_road_split_u",        concat(texturePath, "tileSand_roadSplitN.png")},
            {"sand_road_split_d",        concat(texturePath, "tileSand_roadSplitS.png")},
            {"grass_road_tranisiton_ns", concat(texturePath, "tileGrass_roadTransitionS_dirt.png")},
            {"grass_road_tranisiton_sn", concat(texturePath, "tileGrass_roadTransitionN_dirt.png")},
            {"grass_road_tranisiton_ew", concat(texturePath, "tileGrass_roadTransitionE_dirt.png")},
            {"grass_road_tranisiton_we", concat(texturePath, "tileGrass_roadTransitionW_dirt.png")}
        };
        return assets;
    }
};

class TextureLibrary : public engine::TextureLibrary<GraphicsAssetsList>
{};

}  // namespace graphics
