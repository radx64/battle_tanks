#include "SfmlRenderTarget.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "TextureImpl.hpp"

namespace engine::backends::sfml
{

SfmlRenderTarget::SfmlRenderTarget(sf::RenderTarget& target)
    : target_(target)
{}

void SfmlRenderTarget::drawVertices(const engine::gfx::VertexArray& vertices,
    const engine::gfx::Texture& texture, const engine::gfx::RenderStates& states)
{
    sf::VertexArray sfVertices{sf::Quads, vertices.getVertexCount()};
    for (std::size_t index = 0; index < vertices.getVertexCount(); ++index)
    {
        const auto& source = vertices[index];
        sfVertices[index] = {{source.position.x, source.position.y},
            {source.color.r, source.color.g, source.color.b, source.color.a}, {source.uv.x, source.uv.y}};
    }

    sf::RenderStates sfStates;
    sfStates.texture = texture.impl_->texture_.get();
    sfStates.transform.translate(states.position.x - states.origin.x, states.position.y - states.origin.y);
    target_.draw(sfVertices, sfStates);
}

}  // namespace engine::backends::sfml
