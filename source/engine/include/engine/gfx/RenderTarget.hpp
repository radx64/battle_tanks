#pragma once

#include "engine/gfx/Drawable.hpp"
#include "engine/gfx/Texture.hpp"
#include "engine/gfx/VertexArray.hpp"

#include "engine/gfx/Shape.hpp"
#include "engine/gfx/Sprite.hpp"

#include "engine/Logger.hpp"

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

    void draw(const Sprite& sprite, const RenderStates& states = {})
    {
        (void)sprite;
        (void)states;
        
        engine::Logger logger_{"RenderTarget"};
        logger_.error("RenderTarget::draw(Sprite) called");
    }

    void draw(const Shape& shape, const RenderStates& states = {})
    {
        (void)shape;
        (void)states;
        
        engine::Logger logger_{"RenderTarget"};
        logger_.error("RenderTarget::draw(Shape) called");
    }

    void draw(const RectangleShape& shape, const RenderStates& states = {})
    {
        (void)shape;
        (void)states;
        
        engine::Logger logger_{"RenderTarget"};
        logger_.error("RenderTarget::draw(RectangleShape) called");
    }

    virtual void drawVertices(const VertexArray& vertices, const Texture& texture, const RenderStates& states) = 0;
};

}  // namespace engine::gfx
