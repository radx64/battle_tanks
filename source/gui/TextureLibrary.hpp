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
            {"framedSpriteTest",             concat(texturePath, "framedSpriteTestTex.png")},
            {"button_normal",                concat(texturePath, "classic/button.png")},
            {"button_focus",                 concat(texturePath, "classic/button_focus.png")},
            {"button_hover",                 concat(texturePath, "classic/button_hover.png")},
            {"button_pressed",               concat(texturePath, "classic/button_pressed.png")},
            {"window_normal",                concat(texturePath, "classic/window_normal.png")},
            {"window_inactive",              concat(texturePath, "classic/window_inactive.png")},
            {"window_resize_handle",         concat(texturePath, "classic/window_resize_handle.png")},
        };
        return assets;
    }
};

class TextureLibrary : public engine::TextureLibrary<UserInterfaceAssetsList>
{};

}  // namespace gui
