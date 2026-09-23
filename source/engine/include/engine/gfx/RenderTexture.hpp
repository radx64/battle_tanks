#pragma once

#include <memory>

#include "engine/gfx/Texture.hpp"

namespace engine::gfx
{

class RenderTexture
{
public:
    RenderTexture();
    ~RenderTexture();

    engine::gfx::Texture getTexture() const;

    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace engine::gfx
