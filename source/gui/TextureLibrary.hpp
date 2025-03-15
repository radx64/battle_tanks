#pragma once

#include "engine/Concat.hpp"
#include "engine/TextureLibrary.hpp"

namespace
{
    constexpr char texturePath[] {"../sprites/ui/"};
}

namespace gui
{
class UserInterfaceAssetsList
{
public:
    static const std::map<std::string, std::string>& getAssets()
    {
        using engine::concat;
        static const std::map<std::string, std::string> assets
        {
            {"blue_check_square",            concat(texturePath, "blue/check_square_grey_cross.png")},
            {"red_check_square",             concat(texturePath, "red/check_square_grey_cross.png")},
            {"red_icon_outline_cross",       concat(texturePath, "red/icon_outline_cross.png")},
            {"red_button_hover",             concat(texturePath, "red/button_rectangle_depth_flat.png")},
            {"red_button_pressed",           concat(texturePath, "red/button_rectangle_flat.png")},
            {"red_button_normal",            concat(texturePath, "extra/button_rectangle_depth_line.png")},
            {"framedSpriteTest",             concat(texturePath, "framedSpriteTestTex.png")},

        };
        return assets;
    }
};

class TextureLibrary : public engine::TextureLibrary<UserInterfaceAssetsList>
{};

}  // namespace gui
