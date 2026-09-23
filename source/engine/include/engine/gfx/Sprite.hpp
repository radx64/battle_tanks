#pragma once

#include <memory>

#include "engine/gfx/Texture.hpp"
#include "engine/Rect.hpp"
#include "engine/Vector2.hpp"

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
    void setRotation(const float rotation);
    void setTexture(const Texture& texture);

    void setScale(const float x, const float y);
    void setScale(const engine::Vec2f& scale);

    engine::FloatRect getLocalBounds() const;
    engine::FloatRect getGlobalBounds() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace engine::gfx
