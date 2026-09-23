#pragma once

#include "engine/gfx/RenderStates.hpp"

namespace engine::gfx
{

class RenderTarget;

class Drawable
{
public:
    virtual ~Drawable() = default;

private:
    virtual void draw(RenderTarget& target, const RenderStates& states = RenderStates{}) const = 0;
    friend class RenderTarget;
};

}  // namespace engine::gfx
