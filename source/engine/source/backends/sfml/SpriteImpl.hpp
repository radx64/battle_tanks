#pragma once

#include "engine/gfx/Sprite.hpp"
#include <SFML/Graphics/Sprite.hpp>

namespace engine::gfx
{

class Sprite::Impl
{
public:
    Impl() = default;
    engine::gfx::Texture texture_;
    sf::Sprite sprite_;
};

}  // namespace engine::gfx
