#include "SfmlRenderTarget.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "ShaderImpl.hpp"
#include "SpriteImpl.hpp"
#include "TextImpl.hpp"
#include "TextureImpl.hpp"
#include "backends/sfml/ToSf.hpp"
#include "engine/gfx/Shape.hpp"

namespace engine::backends::sfml
{

namespace
{
sf::Vertex toSf(const engine::gfx::Vertex& vertex)
{
    return sf::Vertex{
        {vertex.position.x, vertex.position.y},
        {vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a},
        {vertex.uv.x, vertex.uv.y}};
}

sf::VertexArray toSf(const engine::gfx::VertexArray& vertices)
{
    sf::VertexArray sfVertices{engine::toSf(vertices.getPrimitiveType()), vertices.getVertexCount()};
    for (std::size_t index = 0; index < vertices.getVertexCount(); ++index)
    {
        sfVertices[index] = toSf(vertices[index]);
    }
    return sfVertices;
}
}  // namespace

SfmlRenderTarget::SfmlRenderTarget(sf::RenderTarget& target)
    : target_(target)
{
}

sf::RenderStates SfmlRenderTarget::toSfStates(const engine::gfx::RenderStates& states) const
{
    sf::RenderStates sfStates;
    sfStates.transform.translate(states.position.x - states.origin.x, states.position.y - states.origin.y);
    if (states.shader != nullptr)
    {
        sfStates.shader = &states.shader->impl_->shader_;
    }
    return sfStates;
}

void SfmlRenderTarget::clear(const engine::gfx::Color& color)
{
    target_.clear(engine::toSf(color));
}

void SfmlRenderTarget::draw(const engine::gfx::Sprite& sprite, const engine::gfx::RenderStates& states)
{
    target_.draw(sprite.impl_->sprite_, toSfStates(states));
}

void SfmlRenderTarget::draw(const engine::gfx::RectangleShape& shape, const engine::gfx::RenderStates& states)
{
    sf::RectangleShape rectangle{engine::toSf(shape.getSize())};
    rectangle.setPosition(shape.getPosition().x, shape.getPosition().y);
    rectangle.setOrigin(shape.getOrigin().x, shape.getOrigin().y);
    rectangle.setRotation(shape.getRotation());
    rectangle.setFillColor(engine::toSf(shape.getFillColor()));
    rectangle.setOutlineColor(engine::toSf(shape.getOutlineColor()));
    rectangle.setOutlineThickness(shape.getOutlineThickness());
    target_.draw(rectangle, toSfStates(states));
}

void SfmlRenderTarget::draw(const engine::gfx::CircleShape& shape, const engine::gfx::RenderStates& states)
{
    sf::CircleShape circle{shape.getRadius(), shape.getPointCount()};
    circle.setPosition(shape.getPosition().x, shape.getPosition().y);
    circle.setOrigin(shape.getOrigin().x, shape.getOrigin().y);
    circle.setRotation(shape.getRotation());
    circle.setFillColor(engine::toSf(shape.getFillColor()));
    circle.setOutlineColor(engine::toSf(shape.getOutlineColor()));
    circle.setOutlineThickness(shape.getOutlineThickness());
    target_.draw(circle, toSfStates(states));
}

void SfmlRenderTarget::draw(const engine::gfx::Text& text, const engine::gfx::RenderStates& states)
{
    target_.draw(text.impl_->text_, toSfStates(states));
}

void SfmlRenderTarget::draw(const engine::gfx::VertexArray& vertices, const engine::gfx::RenderStates& states)
{
    target_.draw(toSf(vertices), toSfStates(states));
}

void SfmlRenderTarget::drawVertices(const engine::gfx::VertexArray& vertices, const engine::gfx::Texture& texture,
    const engine::gfx::RenderStates& states)
{
    auto sfStates = toSfStates(states);
    sfStates.texture = texture.impl_->texture_.get();
    target_.draw(toSf(vertices), sfStates);
}

}  // namespace engine::backends::sfml
