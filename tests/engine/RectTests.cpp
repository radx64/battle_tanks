#include <gtest/gtest.h>

#include "engine/Rect.hpp"

namespace engine
{

using RectUnderTest = Rect<int>;

TEST(RectTests, shouldConstructDefault)
{
    RectUnderTest rectangle;
    EXPECT_EQ(rectangle.left, 0);
    EXPECT_EQ(rectangle.top, 0);
    EXPECT_EQ(rectangle.width, 0);
    EXPECT_EQ(rectangle.height, 0);
}

TEST(RectTests, shouldConstructWithCoordinates)
{
    RectUnderTest rectangle(1, 2, 3, 4);
    EXPECT_EQ(rectangle.left, 1);
    EXPECT_EQ(rectangle.top, 2);
    EXPECT_EQ(rectangle.width, 3);
    EXPECT_EQ(rectangle.height, 4);
}

TEST(RectTests, shouldConstructWithPositionAndSize)
{
    RectUnderTest rectangle(Vector2<int>(1, 2), Vector2<int>(3, 4));
    EXPECT_EQ(rectangle.left, 1);
    EXPECT_EQ(rectangle.top, 2);
    EXPECT_EQ(rectangle.width, 3);
    EXPECT_EQ(rectangle.height, 4);
}

TEST(RectTests, shouldConstructFromRectangleWithDifferentType)
{
    Rect<float> source(1.5f, 2.5f, 3.5f, 4.5f);
    RectUnderTest rectangle(source);
    EXPECT_EQ(rectangle.left, 1);
    EXPECT_EQ(rectangle.top, 2);
    EXPECT_EQ(rectangle.width, 3);
    EXPECT_EQ(rectangle.height, 4);
}

TEST(RectTests, shouldContainPointsInsideItsBounds)
{
    RectUnderTest rectangle(1, 2, 3, 4);
    EXPECT_TRUE(rectangle.contains(1, 2));
    EXPECT_TRUE(rectangle.contains(3, 5));
    EXPECT_FALSE(rectangle.contains(4, 5));
    EXPECT_FALSE(rectangle.contains(3, 6));
    EXPECT_FALSE(rectangle.contains(0, 2));
    EXPECT_FALSE(rectangle.contains(1, 1));
}

TEST(RectTests, shouldContainVectorPoint)
{
    RectUnderTest rectangle(1, 2, 3, 4);
    EXPECT_TRUE(rectangle.contains(Vector2<int>(2, 3)));
    EXPECT_FALSE(rectangle.contains(Vector2<int>(4, 6)));
}

TEST(RectTests, shouldContainPointsForNegativeSize)
{
    RectUnderTest rectangle(4, 6, -3, -4);
    EXPECT_TRUE(rectangle.contains(1, 2));
    EXPECT_TRUE(rectangle.contains(3, 5));
    EXPECT_FALSE(rectangle.contains(4, 5));
    EXPECT_FALSE(rectangle.contains(3, 6));
}

TEST(RectTests, shouldIdentifyIntersectingRectangles)
{
    RectUnderTest rectangle(1, 2, 4, 4);
    EXPECT_TRUE(rectangle.intersects(RectUnderTest(3, 4, 4, 4)));
    EXPECT_FALSE(rectangle.intersects(RectUnderTest(5, 2, 2, 2)));
    EXPECT_FALSE(rectangle.intersects(RectUnderTest(1, 6, 2, 2)));
}

TEST(RectTests, shouldCalculateIntersection)
{
    RectUnderTest rectangle(1, 2, 4, 4);
    RectUnderTest intersection;
    EXPECT_TRUE(rectangle.intersects(RectUnderTest(3, 4, 4, 4), intersection));
    EXPECT_EQ(intersection, RectUnderTest(3, 4, 2, 2));
}

TEST(RectTests, shouldReturnEmptyIntersectionForNonIntersectingRectangles)
{
    RectUnderTest rectangle(1, 2, 4, 4);
    RectUnderTest intersection(9, 9, 9, 9);
    EXPECT_FALSE(rectangle.intersects(RectUnderTest(5, 2, 2, 2), intersection));
    EXPECT_EQ(intersection, RectUnderTest(0, 0, 0, 0));
}

TEST(RectTests, shouldCalculateIntersectionForNegativeSizes)
{
    RectUnderTest rectangle(5, 6, -4, -4);
    RectUnderTest intersection;
    EXPECT_TRUE(rectangle.intersects(RectUnderTest(2, 3, 4, 4), intersection));
    EXPECT_EQ(intersection, RectUnderTest(2, 3, 3, 3));
}

TEST(RectTests, shouldReturnPosition)
{
    RectUnderTest rectangle(1, 2, 3, 4);
    EXPECT_EQ(rectangle.getPosition(), Vector2<int>(1, 2));
}

TEST(RectTests, shouldReturnSize)
{
    RectUnderTest rectangle(1, 2, 3, 4);
    EXPECT_EQ(rectangle.getSize(), Vector2<int>(3, 4));
}

TEST(RectTests, shouldCheckEquality)
{
    RectUnderTest rectangle1(1, 2, 3, 4);
    RectUnderTest rectangle2(1, 2, 3, 4);
    RectUnderTest rectangle3(1, 2, 3, 5);
    EXPECT_TRUE(rectangle1 == rectangle2);
    EXPECT_FALSE(rectangle1 == rectangle3);
}

TEST(RectTests, shouldCheckInequality)
{
    RectUnderTest rectangle1(1, 2, 3, 4);
    RectUnderTest rectangle2(1, 2, 3, 5);
    EXPECT_TRUE(rectangle1 != rectangle2);
    EXPECT_FALSE(rectangle1 != rectangle1);
}

}  // namespace engine
