#pragma once

namespace engine::gfx {class RenderTarget;}

namespace engine::gui
{

class TextDisplayModifier
{
public:
    virtual ~TextDisplayModifier() = default;
    virtual void render(engine::gfx::RenderTarget& renderTarget) = 0;
};

}  // namespace engine::gui
