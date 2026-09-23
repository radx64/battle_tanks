#pragma once

#include <vector>

#include "engine/Rect.hpp"
#include "engine/gfx/Vertex.hpp"

namespace engine::gfx
{

enum class PrimitiveType
{
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan,
    Quads
};

class VertexArray
{
public:
    VertexArray();

    explicit VertexArray(const PrimitiveType type, const size_t vertexCount = 0);

    Vertex& operator[](const size_t index);
    const Vertex& operator[](const size_t index) const;

    void clear();

    void setPrimitiveType(const PrimitiveType type);
    PrimitiveType getPrimitiveType() const;

    void resize(const size_t vertexCount);
    void append(const Vertex& vertex);

    size_t getVertexCount() const;
    engine::FloatRect getBounds() const;

    
private:
    std::vector<Vertex> vertices_;
    PrimitiveType primitiveType_;
};

}  // namespace engine::gfx
