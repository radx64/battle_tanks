#pragma once


#include "engine/ChunkedRenderTextureLayer.hpp"
#include "engine/gfx/RenderTarget.hpp"

namespace game::entity
{

class TracksRenderer
{
public:
    TracksRenderer();
    void addTrackImprint(const int x, const int y, const float angle);
    void draw(engine::gfx::RenderTarget& render_window);
    void clear();

protected:
    engine::ChunkedRenderTextureLayer chunks_;
};

} // namespace game::entity
