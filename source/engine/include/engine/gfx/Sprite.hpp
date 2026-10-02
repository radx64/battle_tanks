#pragma once

#include <memory>

#include "engine/gfx/Color.hpp"
#include "engine/gfx/Texture.hpp"
#include "engine/Rect.hpp"
#include "engine/Vector2.hpp"

namespace engine::backends::sfml
{
class SfmlRenderTarget;
}

namespace engine::gfx
{

class Sprite
{
public:
    Sprite();
    ~Sprite();

    explicit Sprite(const engine::gfx::Texture& texture);

    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;
    Sprite(Sprite&&) noexcept;
    Sprite& operator=(Sprite&&) noexcept;
    
    void setPosition(const engine::Vec2f& position);
    void setPosition(float x, float y);
    engine::Vec2f getPosition() const;
    void setRotation(const float rotation);
    void setOrigin(const engine::Vec2f& origin);
    void setOrigin(float x, float y);
    void setTexture(const Texture& texture, bool resetRect = false);
    void setTextureRect(const engine::IntRect& rectangle);
    void setColor(const Color& color);

    void setScale(const float x, const float y);
    void setScale(const engine::Vec2f& scale);

    engine::FloatRect getLocalBounds() const;
    engine::FloatRect getGlobalBounds() const;

private:
    friend class engine::backends::sfml::SfmlRenderTarget;

    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace engine::gfx
