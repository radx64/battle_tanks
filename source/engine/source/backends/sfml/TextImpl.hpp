#pragma once

#include "engine/gfx/Text.hpp"

#include <SFML/Graphics/Text.hpp>

namespace engine::gfx
{

class Text::Impl
{
public:
    sf::Text text_;
    const Font* font_{nullptr};
};

}  // namespace engine::gfx
