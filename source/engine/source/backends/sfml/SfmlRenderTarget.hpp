#pragma once

#include <SFML/Graphics/RenderStates.hpp>

#include "engine/gfx/RenderTarget.hpp"

namespace sf
{
class RenderTarget;
}

namespace engine::backends::sfml
{

class SfmlRenderTarget final : public engine::gfx::RenderTarget
{
public:
    explicit SfmlRenderTarget(sf::RenderTarget& target);

    void clear(const engine::gfx::Color& color) override;
    void draw(const engine::gfx::Sprite& sprite, const engine::gfx::RenderStates& states = {}) override;
    void draw(const engine::gfx::RectangleShape& shape, const engine::gfx::RenderStates& states = {}) override;
    void draw(const engine::gfx::CircleShape& shape, const engine::gfx::RenderStates& states = {}) override;
    void draw(const engine::gfx::Text& text, const engine::gfx::RenderStates& states = {}) override;
    void draw(const engine::gfx::VertexArray& vertices, const engine::gfx::RenderStates& states = {}) override;
    void drawVertices(const engine::gfx::VertexArray& vertices, const engine::gfx::Texture& texture,
        const engine::gfx::RenderStates& states) override;

private:
    sf::RenderStates toSfStates(const engine::gfx::RenderStates& states) const;

    sf::RenderTarget& target_;
};

}  // namespace engine::backends::sfml
