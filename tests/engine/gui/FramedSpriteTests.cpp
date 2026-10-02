#include <gtest/gtest.h>

#include "engine/gfx/RenderTarget.hpp"
#include "engine/gui/FramedSprite.hpp"

namespace
{

class RecordingTarget : public engine::gfx::RenderTarget
{
public:
    using engine::gfx::RenderTarget::draw;

    void clear(const engine::gfx::Color&) override {}
    void draw(const engine::gfx::Sprite&, const engine::gfx::RenderStates&) override {}
    void draw(const engine::gfx::RectangleShape&, const engine::gfx::RenderStates&) override {}
    void draw(const engine::gfx::CircleShape&, const engine::gfx::RenderStates&) override {}
    void draw(const engine::gfx::Text&, const engine::gfx::RenderStates&) override {}
    void draw(const engine::gfx::VertexArray&, const engine::gfx::RenderStates&) override {}
    void drawVertices(const engine::gfx::VertexArray& vertices, const engine::gfx::Texture& texture,
        const engine::gfx::RenderStates& states) override
    {
        drawn = true;
        captured = vertices;
        capturedStates = states;
        capturedTexture = &texture;
    }

    bool drawn{false};
    engine::gfx::VertexArray captured;
    engine::gfx::RenderStates capturedStates{};
    const engine::gfx::Texture* capturedTexture{nullptr};
};

engine::gui::FramedSprite::LayoutConfig testLayout()
{
    return engine::gui::FramedSprite::LayoutConfig{
        .cornerSizes = {
            .topLeft = {4.f, 4.f},
            .bottomRight = {4.f, 4.f},
        },
        .uvs = {
            .topLeft = {0.f, 0.f, 2.f, 2.f},
            .topRight = {4.f, 0.f, 2.f, 2.f},
            .bottomLeft = {0.f, 4.f, 2.f, 2.f},
            .bottomRight = {4.f, 4.f, 2.f, 2.f},
        },
    };
}

void expectVertex(const engine::gfx::Vertex& vertex, float x, float y, float u, float v)
{
    EXPECT_FLOAT_EQ(vertex.position.x, x);
    EXPECT_FLOAT_EQ(vertex.position.y, y);
    EXPECT_FLOAT_EQ(vertex.uv.x, u);
    EXPECT_FLOAT_EQ(vertex.uv.y, v);
}

}  // namespace

namespace engine::gui
{

TEST(FramedSpriteTests, shouldNotDrawWithoutATexture)
{
    FramedSprite sprite{testLayout()};
    sprite.setSize({20.f, 16.f});
    RecordingTarget target;

    target.draw(sprite);

    EXPECT_FALSE(target.drawn);
}

TEST(FramedSpriteTests, shouldSliceIntoNineQuadsAndPassTransform)
{
    const engine::gfx::Texture texture;
    FramedSprite sprite{testLayout()};
    sprite.setTexture(texture);
    sprite.setSize({20.f, 16.f});
    sprite.setPosition({10.f, 30.f});
    sprite.setOrigin({1.f, 2.f});

    RecordingTarget target;
    target.draw(sprite);

    ASSERT_TRUE(target.drawn);
    EXPECT_EQ(target.capturedTexture, &texture);
    EXPECT_EQ(target.captured.getPrimitiveType(), engine::gfx::PrimitiveType::Quads);
    ASSERT_EQ(target.captured.getVertexCount(), 36u);
    EXPECT_FLOAT_EQ(target.capturedStates.position.x, 10.f);
    EXPECT_FLOAT_EQ(target.capturedStates.position.y, 30.f);
    EXPECT_FLOAT_EQ(target.capturedStates.origin.x, 1.f);
    EXPECT_FLOAT_EQ(target.capturedStates.origin.y, 2.f);

    expectVertex(target.captured[0], 0.f, 0.f, 0.f, 0.f);
    expectVertex(target.captured[2], 4.f, 4.f, 2.f, 2.f);
    expectVertex(target.captured[5], 16.f, 0.f, 4.f, 0.f);
    expectVertex(target.captured[9], 20.f, 0.f, 6.f, 0.f);
    expectVertex(target.captured[16], 4.f, 4.f, 2.f, 2.f);
    expectVertex(target.captured[18], 16.f, 12.f, 4.f, 4.f);
    expectVertex(target.captured[34], 20.f, 16.f, 6.f, 6.f);

    EXPECT_EQ(target.captured[0].color.r, 255);
    EXPECT_EQ(target.captured[0].color.g, 255);
    EXPECT_EQ(target.captured[0].color.b, 255);
    EXPECT_EQ(target.captured[0].color.a, 255);
}

TEST(FramedSpriteTests, shouldTintEveryVertex)
{
    const engine::gfx::Texture texture;
    FramedSprite sprite{testLayout()};
    sprite.setTexture(texture);
    sprite.setSize({20.f, 16.f});
    sprite.setColor(engine::gfx::Color{10, 20, 30, 40});

    RecordingTarget target;
    target.draw(sprite);

    ASSERT_EQ(target.captured.getVertexCount(), 36u);
    for (std::size_t index = 0; index < target.captured.getVertexCount(); ++index)
    {
        EXPECT_EQ(target.captured[index].color.r, 10);
        EXPECT_EQ(target.captured[index].color.g, 20);
        EXPECT_EQ(target.captured[index].color.b, 30);
        EXPECT_EQ(target.captured[index].color.a, 40);
    }
}

}  // namespace engine::gui
