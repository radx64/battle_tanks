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
            {"editbox_active",               concat(texturePath, "classic/editbox_active.png")},
            {"editbox_inactive",             concat(texturePath, "classic/editbox_inactive.png")},
            {"window_active",                concat(texturePath, "classic/window_active.png")},
            {"window_inactive",              concat(texturePath, "classic/window_inactive.png")},
            {"window_resize_handle",         concat(texturePath, "classic/window_resize_handle.png")},
            {"progressbar_background",       concat(texturePath, "classic/progressbar_background.png")},
        };
        return assets;
    }
};

class TextureLibrary : public engine::TextureLibrary<UserInterfaceAssetsList>
{};

}  // namespace gui
