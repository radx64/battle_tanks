#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "engine/gfx/Texture.hpp"
#include "engine/gfx/Sprite.hpp"

#include "engine/gui/Widget.hpp"

namespace engine::gui
{

class Image : public Widget
{
public:
    static std::unique_ptr<Image> create(const engine::gfx::Texture& texture);

    void onRender(engine::gfx::RenderTarget& renderTarget) override;
    void setTexture(const engine::gfx::Texture& texture);

protected:
    Image(const engine::gfx::Texture& texture);

    void onPositionChange() override;
    void onSizeChange() override;

    engine::gfx::Sprite sprite_;
    const engine::gfx::Texture& texture_;
};

}  // namespace engine::gui
