#pragma once

#include <string>

#include "engine/Application.hpp"
#include "engine/gfx/RenderTexture.hpp"
#include "engine/gfx/Shader.hpp"
#include "engine/gfx/Sprite.hpp"

namespace bios
{

class Application : public engine::Application
{
public:
    Application();

protected:
    void onInit() override;
    void onClose() override;
    void onEvent(engine::events::Event& event) override;
    void onUpdate(float time_step) override;
    void onRender() override;

private:
    void drawScene(engine::gfx::RenderTarget& target);
    void drawPostScreen(engine::gfx::RenderTarget& target, float t);
    void drawBootScreen(engine::gfx::RenderTarget& target, float t);

    void drawText(engine::gfx::RenderTarget& target, const std::string& text,
        float x, float y, engine::gfx::Color color = engine::gfx::Color(0xC0, 0xC0, 0xC0));

    engine::gfx::Font font_;
    engine::gfx::Text text_;

    engine::gfx::RenderTexture screen_;
    engine::gfx::Sprite screenSprite_;
    engine::gfx::Shader crtShader_;
    bool crtAvailable_{false};

    engine::Stopwatch bootClock_;
    float elapsed_{0.f};
    static constexpr float BOOT_DURATION_{25.f};
};

}  // namespace bios
