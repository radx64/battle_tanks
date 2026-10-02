#include <gtest/gtest.h>

#include "engine/gfx/Shape.hpp"

namespace engine::gfx
{

TEST(ShapeTests, shouldPlaceGlobalBoundsAtPositionMinusOrigin)
{
    RectangleShape rectangle{engine::Vec2f{40.f, 20.f}};
    rectangle.setPosition(100.f, 80.f);
    rectangle.setOrigin(10.f, 5.f);
    rectangle.setRotation(45.f);

    const auto bounds = rectangle.getGlobalBounds();

    EXPECT_FLOAT_EQ(bounds.left, 90.f);
    EXPECT_FLOAT_EQ(bounds.top, 75.f);
    EXPECT_FLOAT_EQ(bounds.width, 40.f);
    EXPECT_FLOAT_EQ(bounds.height, 20.f);
    EXPECT_FLOAT_EQ(rectangle.getRotation(), 45.f);
}

TEST(CircleShapeTests, shouldStoreRadiusAsDiameter)
{
    CircleShape circle{12.f};

    EXPECT_EQ(circle.getPointCount(), 30u);
    EXPECT_FLOAT_EQ(circle.getRadius(), 12.f);
    EXPECT_FLOAT_EQ(circle.getSize().x, 24.f);
    EXPECT_FLOAT_EQ(circle.getSize().y, 24.f);

    circle.setRadius(3.f);
    circle.setPointCount(8);
    circle.setPosition({30.f, 10.f});
    circle.setOrigin({12.f, 12.f});

    const auto bounds = circle.getGlobalBounds();

    EXPECT_EQ(circle.getPointCount(), 8u);
    EXPECT_FLOAT_EQ(circle.getRadius(), 3.f);
    EXPECT_FLOAT_EQ(bounds.left, 18.f);
    EXPECT_FLOAT_EQ(bounds.top, -2.f);
    EXPECT_FLOAT_EQ(bounds.width, 6.f);
    EXPECT_FLOAT_EQ(bounds.height, 6.f);
}

}  // namespace engine::gfx
