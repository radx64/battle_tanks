#pragma once

#include "engine/gui/Widget.hpp"
#include "engine/gui/FramedSprite.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Shape.hpp"
#include "engine/gfx/Texture.hpp"

namespace engine::gui { class Label; }

namespace engine::gui
{

class ProgressBar : public Widget
{
public:
    static std::unique_ptr<ProgressBar> create();
    void setRange(const float min, const float max);
    void setValue(const float value);
    float getValue() const;

protected:
    ProgressBar();
    void setTextLabel(const float value);
    void onPositionChange() override;
    void onSizeChange() override;
    void onRender(engine::gfx::RenderTarget& renderTarget) override;

    void recalculateSize();

    float min_{};
    float max_{};
    float value_{};
    float progress_{};

    engine::gui::FramedSprite background_;
    const engine::gfx::Texture& normalTexture_;

    engine::gfx::RectangleShape bar_;
    engine::gui::Label* text_;
};

}  // namespace
