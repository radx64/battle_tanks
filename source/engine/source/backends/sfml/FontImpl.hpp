#pragma once

#include "engine/gfx/Font.hpp"

#include <SFML/Graphics/Font.hpp>

namespace engine::gfx
{

class Font::Impl
{
public:
    sf::Font font_;
};

}  // namespace engine::gfx
