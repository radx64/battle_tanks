#pragma once

#include <string_view>

#include "engine/Particle.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Text.hpp"

namespace game::particle
{

class Say : public engine::Particle
{
public:
    Say(const std::string_view text, float x, float y);
    virtual ~Say() = default;

    void draw(engine::gfx::RenderTarget& render_window) override;

protected:
    void onUpdate(float time_step) override;

private:
    float lifetime_;
    engine::gfx::Text text_;
};

}  // namespace game::particle
