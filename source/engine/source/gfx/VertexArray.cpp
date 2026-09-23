#include "engine/gfx/VertexArray.hpp"

namespace engine::gfx
{

VertexArray::VertexArray()
: vertices_{}
, primitiveType_(PrimitiveType::Points)
{}

VertexArray::VertexArray(const PrimitiveType type, const size_t vertexCount)
: vertices_{vertexCount}
, primitiveType_{type}
{}

Vertex& VertexArray::operator[](const size_t index)
{
    return vertices_[index];
}

const Vertex& VertexArray::operator[](const size_t index) const
{
    return vertices_[index];
}

void VertexArray::clear()
{
    vertices_.clear();
}

void VertexArray::setPrimitiveType(const PrimitiveType type)
{
    primitiveType_ = type;
}

PrimitiveType VertexArray::getPrimitiveType() const
{
    return primitiveType_;
}

void VertexArray::resize(const size_t vertexCount)
{
    vertices_.resize(vertexCount);
}

void VertexArray::append(const Vertex& vertex)
{
    vertices_.push_back(vertex);
}

size_t VertexArray::getVertexCount() const
{
    return vertices_.size();
}

engine::FloatRect VertexArray::getBounds() const
{
    if (vertices_.empty())
    {
        return engine::FloatRect{};
    }

    auto min = vertices_.front().position;
    auto max = vertices_.front().position;

    for (const auto& vertex : vertices_)
    {
        min.x = std::min(min.x, vertex.position.x);
        min.y = std::min(min.y, vertex.position.y);
        max.x = std::max(max.x, vertex.position.x);
        max.y = std::max(max.y, vertex.position.y);
    }

    return engine::FloatRect{min, max - min};
}

}  // namespace engine::gfx
