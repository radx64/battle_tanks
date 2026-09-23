#pragma once

#include "engine/gfx/Texture.hpp"

#include "SFML/Graphics/Texture.hpp"

namespace engine::gfx
{

class Texture::Impl
{
public:
    std::shared_ptr<sf::Texture> texture_ = std::make_shared<sf::Texture>();
};

}  // namespace engine::gfx
