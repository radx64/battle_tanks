#pragma once

#include "engine/gfx/RenderTexture.hpp"

#include "SFML/Graphics/RenderTexture.hpp"

namespace engine::gfx
{

class RenderTexture::Impl
{
public:
    std::shared_ptr<sf::RenderTexture> render_texture_ = std::make_shared<sf::RenderTexture>();
};

}  // namespace engine::gfx
