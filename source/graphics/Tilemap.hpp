#pragma once

#include <memory>

#include "engine/gfx/RenderTarget.hpp"


namespace graphics
{

class Tilemap
{
public:
    Tilemap();
    ~Tilemap();

    void draw(engine::gfx::RenderTarget& render_window);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace graphics
