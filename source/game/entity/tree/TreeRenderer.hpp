#pragma once

#include "engine/Renderer.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Sprite.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity { class Tree; }

namespace game::entity
{
class TreeRenderer : public engine::Renderer
{
public:
    TreeRenderer(Tree* tree, engine::gfx::Texture& treeBody);
    void draw(engine::gfx::RenderTarget& render_window);

protected:
    Tree* tree_;
    engine::gfx::Sprite treeSprite_;
};


}  // namespace game::entity 
