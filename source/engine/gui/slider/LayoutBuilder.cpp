#include "engine/gui/slider/LayoutBuilder.hpp"

namespace engine::gui::slider
{

engine::gui::FramedSprite::LayoutConfig buildLayoutConfig(const sf::Vector2f& cornerSizes, const engine::gui::FramedSprite::LayoutConfig::UVs& uvs)
{
    engine::gui::FramedSprite::LayoutConfig layoutConfig{
        .cornerSizes = 
        {
            .topLeft        = {cornerSizes.x, cornerSizes.y},
            .bottomRight    = {cornerSizes.x, cornerSizes.y}
        },
        .uvs = uvs
    };

    return layoutConfig;
} 

engine::gui::FramedSprite::LayoutConfig::UVs buildUVsForTrackTexture()
{
    return engine::gui::FramedSprite::LayoutConfig::UVs
    {
        .topLeft        = {0.0f,   0.0f,  2.0f, 2.0f},
        .topRight       = {4.0f,   0.0f,  2.0f, 2.0f},
        .bottomLeft     = {0.0f,   4.0f,  2.0f, 2.0f},
        .bottomRight    = {4.0f,   4.0f,  2.0f, 2.0f},
    };
}

engine::gui::FramedSprite::LayoutConfig buildLayoutConfigForTrackTexture()
{
    static auto layout = buildLayoutConfig({4.f, 4.f}, buildUVsForTrackTexture());
    return layout;
}


engine::gui::FramedSprite::LayoutConfig::UVs buildUVsForThumbTexture()
{
    return engine::gui::FramedSprite::LayoutConfig::UVs
    {
        .topLeft        = {0.0f,   0.0f,  2.0f, 2.0f},
        .topRight       = {4.0f,   0.0f,  2.0f, 2.0f},
        .bottomLeft     = {0.0f,   4.0f,  2.0f, 2.0f},
        .bottomRight    = {4.0f,   4.0f,  2.0f, 2.0f},
    };
}

engine::gui::FramedSprite::LayoutConfig buildLayoutConfigForThumbTexture()
{
    static auto layout = buildLayoutConfig({4.f, 4.f}, buildUVsForThumbTexture());
    return layout;
}

}  // namespace engine::gui::slider 
