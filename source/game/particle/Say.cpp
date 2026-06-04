#include "game/particle/Say.hpp"

#include <algorithm>

#include "gui/FontLibrary.hpp"

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
    velocity_ = sf::Vector2f{0.f, SAY_VELOCITY_Y};

    text_.setFont(gui::FontLibrary::get("armata"));
    text_.setString(std::string{text});
    text_.setCharacterSize(SAY_CHARACTER_SIZE);
    text_.setFillColor(sf::Color::White);
    text_.setOutlineColor(sf::Color::Black);
    text_.setOutlineThickness(1.f);
    text_.setStyle(sf::Text::Bold);

    const auto bounds = text_.getLocalBounds();
    text_.setOrigin(bounds.left + bounds.width * 0.5f,
                     bounds.top + bounds.height * 0.5f);
}

void Say::draw(sf::RenderWindow& renderWindow)
{
    const float alphaValue = std::max(0.f, 1.f - lifetime_ / SAY_LIFETIME);
    sf::Color fillColor = text_.getFillColor();
    fillColor.a = static_cast<sf::Uint8>(255.f * alphaValue);
    text_.setFillColor(fillColor);

    sf::Color outlineColor = text_.getOutlineColor();
    outlineColor.a = fillColor.a;
    text_.setOutlineColor(outlineColor);

    text_.setPosition(position_);
    renderWindow.draw(text_);
}

void Say::onUpdate(float timeStep)
{
    lifetime_ += timeStep;
    if (lifetime_ > SAY_LIFETIME)
    {
        kill();
    }
}

}  // namespace game::particle
