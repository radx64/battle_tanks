#pragma once

namespace engine::gfx { class RenderTarget; }

namespace engine 
{

class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual void draw(engine::gfx::RenderTarget& renderTarget) = 0;
};

}  // namespace engine
