#pragma once

#include "engine/gfx/Shader.hpp"

#include <SFML/Graphics/Shader.hpp>

namespace engine::gfx
{

class Shader::Impl
{
public:
    sf::Shader shader_;
};

}  // namespace engine::gfx
