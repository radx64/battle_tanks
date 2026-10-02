#pragma once

#include <vector>

#include "engine/Vector2.hpp"
#include "engine/gfx/Color.hpp"
#include "engine/gfx/RenderTarget.hpp"


namespace graphics::drawtools
{

void drawLine(engine::gfx::RenderTarget& window, int x1, int y1, int x2, int y2, engine::gfx::Color color);
void drawTarget(engine::gfx::RenderTarget& window, int x, int y);
void drawWaypoints(engine::gfx::RenderTarget& window, std::vector<engine::Vec2i>& waypoints);

}  // namespeace graphics::drawtools


