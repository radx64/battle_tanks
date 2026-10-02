#pragma once

#include "engine/gfx/Color.hpp"
#include "engine/gfx/Drawable.hpp"
#include "engine/gfx/RenderStates.hpp"
#include "engine/gfx/Shape.hpp"
#include "engine/gfx/Sprite.hpp"
#include "engine/gfx/Text.hpp"
#include "engine/gfx/Texture.hpp"
#include "engine/gfx/VertexArray.hpp"

namespace engine::gfx
{

class RenderTarget
{
public:
    virtual ~RenderTarget() = default;

    void draw(const Drawable& drawable, const RenderStates& states = {})
    {
        drawable.draw(*this, states);
    }

    virtual void clear(const Color& color) = 0;

    virtual void draw(const Sprite& sprite, const RenderStates& states = {}) = 0;
    virtual void draw(const RectangleShape& shape, const RenderStates& states = {}) = 0;
    virtual void draw(const CircleShape& shape, const RenderStates& states = {}) = 0;
    virtual void draw(const Text& text, const RenderStates& states = {}) = 0;
    virtual void draw(const VertexArray& vertices, const RenderStates& states = {}) = 0;
    virtual void drawVertices(const VertexArray& vertices, const Texture& texture, const RenderStates& states) = 0;
};

}  // namespace engine::gfx
