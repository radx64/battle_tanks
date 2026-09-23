#pragma once

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

    void drawVertices(const engine::gfx::VertexArray& vertices, const engine::gfx::Texture& texture,
        const engine::gfx::RenderStates& states) override;

private:
    sf::RenderTarget& target_;
};

}  // namespace engine::backends::sfml
