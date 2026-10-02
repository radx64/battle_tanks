#include <gtest/gtest.h>

#include "engine/gfx/VertexArray.hpp"

namespace engine::gfx
{

TEST(VertexArrayTests, shouldReportEmptyBoundsWhenThereAreNoVertices)
{
    const VertexArray vertices{PrimitiveType::Lines};

    EXPECT_EQ(vertices.getVertexCount(), 0u);
    EXPECT_EQ(vertices.getPrimitiveType(), PrimitiveType::Lines);

    const auto bounds = vertices.getBounds();
    EXPECT_FLOAT_EQ(bounds.left, 0.f);
    EXPECT_FLOAT_EQ(bounds.top, 0.f);
    EXPECT_FLOAT_EQ(bounds.width, 0.f);
    EXPECT_FLOAT_EQ(bounds.height, 0.f);
}

TEST(VertexArrayTests, shouldBoundTheVertexPositions)
{
    VertexArray vertices{PrimitiveType::Quads};
    vertices.append(Vertex{engine::Vec2f{10.f, 30.f}, Color::Red});
    vertices.append(Vertex{engine::Vec2f{-4.f, 8.f}, Color::White});
    vertices.append(Vertex{engine::Vec2f{20.f, 2.f}, Color::Blue});

    const auto bounds = vertices.getBounds();

    EXPECT_EQ(vertices.getVertexCount(), 3u);
    EXPECT_FLOAT_EQ(bounds.left, -4.f);
    EXPECT_FLOAT_EQ(bounds.top, 2.f);
    EXPECT_FLOAT_EQ(bounds.width, 24.f);
    EXPECT_FLOAT_EQ(bounds.height, 28.f);
}

TEST(VertexArrayTests, shouldResizeWithWhiteVerticesAndClearThem)
{
    VertexArray vertices;
    vertices.setPrimitiveType(PrimitiveType::TriangleStrip);
    vertices.resize(2);

    EXPECT_EQ(vertices.getPrimitiveType(), PrimitiveType::TriangleStrip);
    EXPECT_EQ(vertices.getVertexCount(), 2u);
    EXPECT_EQ(vertices[0].color.r, 255);
    EXPECT_EQ(vertices[0].color.g, 255);
    EXPECT_EQ(vertices[0].color.b, 255);
    EXPECT_EQ(vertices[0].color.a, 255);
    EXPECT_EQ(vertices[1].color.r, 255);

    vertices[1].position = {3.f, 4.f};
    vertices.clear();

    EXPECT_EQ(vertices.getVertexCount(), 0u);
    EXPECT_FLOAT_EQ(vertices.getBounds().width, 0.f);
}

}  // namespace engine::gfx
