#include "game/particle/Say.hpp"

#include <algorithm>

#include "engine/gui/FontLibrary.hpp"

namespace game::particle
{
constexpr float SAY_LIFETIME = 5.f;
constexpr float SAY_VELOCITY_Y = -20.f;
constexpr unsigned SAY_CHARACTER_SIZE = 12u;

Say::Say(const std::string_view text, float x, float y)
: lifetime_{0.f}
{
    position_.x = x;
    position_.y = y;
    velocity_ = engine::Vec2f{0.f, SAY_VELOCITY_Y};

    text_.setFont(engine::gui::FontLibrary::get("armata"));
    text_.setString(std::string{text});
    text_.setCharacterSize(SAY_CHARACTER_SIZE);
    text_.setFillColor(engine::gfx::Color::White);
    text_.setOutlineColor(engine::gfx::Color::Black);
    text_.setOutlineThickness(1.f);
    text_.setStyle(engine::gfx::Text::Bold);

    const auto bounds = text_.getLocalBounds();
    text_.setOrigin(bounds.left + bounds.width * 0.5f,
                     bounds.top + bounds.height * 0.5f);
}

void Say::draw(engine::gfx::RenderTarget& render_window)
{
    const float alphaValue = std::max(0.f, 1.f - lifetime_ / SAY_LIFETIME);
    engine::gfx::Color fillColor = text_.getFillColor();
    fillColor.a = static_cast<std::uint8_t>(255.f * alphaValue);
    text_.setFillColor(fillColor);

    engine::gfx::Color outlineColor = text_.getOutlineColor();
    outlineColor.a = fillColor.a;
    text_.setOutlineColor(outlineColor);

    text_.setPosition(position_);
   render_window.draw(text_);
}

void Say::onUpdate(float time_step)
{
    lifetime_ += time_step;
    if (lifetime_ > SAY_LIFETIME)
    {
        kill();
    }
}

}  // namespace game::particle
