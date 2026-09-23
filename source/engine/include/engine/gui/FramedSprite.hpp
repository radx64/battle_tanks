#pragma once

#include <cstddef>

#include "engine/Rect.hpp"
#include "engine/Vector2.hpp"
#include "engine/gfx/Color.hpp"
#include "engine/gfx/Drawable.hpp"
#include "engine/gfx/Texture.hpp"
#include "engine/gfx/Transformable.hpp"
#include "engine/gfx/VertexArray.hpp"

/*

FramedSprite helps drawing ui sprites
slicing them into 9 pieces. When resized
it will keep the corners and stretch the
middle parts.

+---+-------------------+---+
|TL |        TM         |TR |
+---+-------------------+---+
|   |                   |   |
|   |                   |   |
|   |                   |   |
|LM |      CENTER       |RM |
|   |                   |   |
|   |                   |   |
|   |                   |   |
+---+-------------------+---+
|BL |        BM         |BR |
+---+-------------------+---+
Center and middle parts are dynamically
calculated based on corner sizes.

Above layout is then transformed to UVs
coordinates on the texture, for each vertex.
*/


namespace engine::gui
{

class FramedSprite : public engine::gfx::Drawable, public engine::gfx::Transformable
{
public:

    struct LayoutConfig
    {
        struct CornersSizes
        {
            engine::Vec2f topLeft;
            engine::Vec2f bottomRight;
        };
        struct UVs
        {
            engine::FloatRect topLeft;
            engine::FloatRect topRight;
            engine::FloatRect bottomLeft;
            engine::FloatRect bottomRight;
        };

        CornersSizes cornerSizes; // corner size in pixels
        UVs uvs; // UVs for each corner size in pixels
    };

    FramedSprite(const LayoutConfig& layoutConfig);
    void setSize(const engine::Vec2f& size);
    const engine::Vec2f& getSize() const;
    void setTexture(const engine::gfx::Texture& texture);
    void setLayoutConfig(const LayoutConfig& layoutConfig);
    const engine::gfx::Texture& getTexture() const;
    void recalculateVerticies();
    void setColor(const engine::gfx::Color& color);

    template <typename ColorLike>
    void setColor(const ColorLike& color)
    {
        setColor(engine::gfx::Color{color.r, color.g, color.b, color.a});
    }

protected:
    void setVertex(size_t index, const engine::Vec2f& position, const engine::Vec2f& uv);
    void setQuad(size_t index, const engine::FloatRect& rect, const engine::FloatRect& uv);

    engine::gfx::VertexArray verticies_;
    engine::Vec2f size_;
    LayoutConfig layoutConfig_;
    const engine::gfx::Texture* texture_;

    void draw(engine::gfx::RenderTarget& target, const engine::gfx::RenderStates& states) const override;
};

} // namespace engine::gui
