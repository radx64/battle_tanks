#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "engine/Rect.hpp"
#include "engine/Vector2.hpp"
#include "engine/gfx/Color.hpp"
#include "engine/gfx/Font.hpp"

namespace engine::backends::sfml
{
class SfmlRenderTarget;
}

namespace engine::gfx
{

class Text
{
public:
    Text();
    ~Text();

    Text(const Text&) = delete;
    Text& operator=(const Text&) = delete;
    Text(Text&&) noexcept;
    Text& operator=(Text&&) noexcept;

    void setFont(const Font& font);
    const Font* getFont() const;

    void setCharacterSize(std::uint32_t size);
    std::uint32_t getCharacterSize() const;

    void setString(std::string_view text);
    std::string getString() const;

    enum Style : std::uint32_t
    {
        Regular = 0,
        Bold = 1 << 0,
        Italic = 1 << 1,
        Underlined = 1 << 2,
        StrikeThrough = 1 << 3,
    };

    void setStyle(std::uint32_t style);
    void setFillColor(const Color& color);
    Color getFillColor() const;
    void setOutlineColor(const Color& color);
    Color getOutlineColor() const;
    void setOutlineThickness(float thickness);
    void setOrigin(float x, float y);
    void setPosition(const engine::Vec2f& position);
    void setPosition(float x, float y);

    engine::FloatRect getLocalBounds() const;

    class Impl;

private:
    friend class engine::backends::sfml::SfmlRenderTarget;

    std::unique_ptr<Impl> impl_;
};

}  // namespace engine::gfx
