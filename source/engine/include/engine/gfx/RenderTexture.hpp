#pragma once

#include <memory>

#include "engine/Vector2.hpp"
#include "engine/gfx/Color.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Texture.hpp"

namespace engine::gfx
{

class RenderTexture : public RenderTarget
{
public:
    RenderTexture();
    ~RenderTexture() override;

    RenderTexture(const RenderTexture&) = delete;
    RenderTexture& operator=(const RenderTexture&) = delete;
    RenderTexture(RenderTexture&&) noexcept;
    RenderTexture& operator=(RenderTexture&&) noexcept;

    bool create(unsigned int width, unsigned int height);
    void display();
    engine::Vec2u getSize() const;
    engine::gfx::Texture getTexture() const;

    void clear(const Color& color) override;
    void draw(const Sprite& sprite, const RenderStates& states = {}) override;
    void draw(const RectangleShape& shape, const RenderStates& states = {}) override;
    void draw(const CircleShape& shape, const RenderStates& states = {}) override;
    void draw(const Text& text, const RenderStates& states = {}) override;
    void draw(const VertexArray& vertices, const RenderStates& states = {}) override;
    void drawVertices(const VertexArray& vertices, const Texture& texture, const RenderStates& states) override;

    class Impl;

private:
    std::unique_ptr<Impl> impl_;
};

}  // namespace engine::gfx
