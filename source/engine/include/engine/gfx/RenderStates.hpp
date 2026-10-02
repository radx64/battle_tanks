#pragma once

#include "engine/Vector2.hpp"

namespace engine::gfx
{

class Shader;

struct RenderStates
{
    engine::Vec2f position{};
    engine::Vec2f origin{};
    const Shader* shader{nullptr};
};

}  // namespace engine::gfx
