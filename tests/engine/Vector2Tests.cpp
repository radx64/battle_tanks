#include <gtest/gtest.h>

#include "engine/Vector2.hpp"

namespace engine
{

using VectorUnderTest = Vector2<int>;

TEST(Vector2Tests, shouldConstructDefault)
{
    VectorUnderTest vector;
    EXPECT_EQ(vector.x, 0);
    EXPECT_EQ(vector.y, 0);
}

TEST(Vector2Tests, shouldConstructWithCoordinates)
{
    VectorUnderTest vector(1, 2);
    EXPECT_EQ(vector.x, 1);
    EXPECT_EQ(vector.y, 2);
}

TEST(Vector2Tests, shouldConstructFromAnotherVector)
{
    Vector2<float> vector1(1.5f, 2.5f);
    VectorUnderTest vector2(vector1);
    EXPECT_EQ(vector2.x, 1);
    EXPECT_EQ(vector2.y, 2);
}

TEST(Vector2Tests, shouldAddVectors)
{
    VectorUnderTest vector1(1, 2);
    VectorUnderTest vector2(3, 4);
    VectorUnderTest result = vector1 + vector2;
    EXPECT_EQ(result.x, 4);
    EXPECT_EQ(result.y, 6);
}

TEST(Vector2Tests, shouldSubtractVectors)
{
    VectorUnderTest vector1(5, 6);
    VectorUnderTest vector2(3, 4);
    VectorUnderTest result = vector1 - vector2;
    EXPECT_EQ(result.x, 2);
    EXPECT_EQ(result.y, 2);
}

TEST(Vector2Tests, shouldMultiplyVectorByScalar)
{
    VectorUnderTest vector(2, 3);
    int scalar = 4;
    VectorUnderTest result = vector * scalar;
    EXPECT_EQ(result.x, 8);
    EXPECT_EQ(result.y, 12);
}

TEST(Vector2Tests, shouldDivideVectorByScalar)
{
    VectorUnderTest vector(8, 12);
    int scalar = 4;
    VectorUnderTest result = vector / scalar;
    EXPECT_EQ(result.x, 2);
    EXPECT_EQ(result.y, 3);
}

TEST(Vector2Tests, shouldCheckEquality)
{
    VectorUnderTest vector1(1, 2);
    VectorUnderTest vector2(1, 2);
    VectorUnderTest vector3(3, 4);
    EXPECT_TRUE(vector1 == vector2);
    EXPECT_FALSE(vector1 == vector3);
}

TEST(Vector2Tests, shouldCheckInequality)
{
    VectorUnderTest vector1(1, 2);
    VectorUnderTest vector2(3, 4);
    EXPECT_TRUE(vector1 != vector2);
    EXPECT_FALSE(vector1 != vector1);
}

TEST(Vector2Tests, shouldNegateVector)
{
    VectorUnderTest vector(1, -2);
    VectorUnderTest result = -vector;
    EXPECT_EQ(result.x, -1);
    EXPECT_EQ(result.y, 2);
}

TEST(Vector2Tests, shouldAddAssignVectors)
{
    VectorUnderTest vector1(1, 2);
    VectorUnderTest vector2(3, 4);
    vector1 += vector2;
    EXPECT_EQ(vector1.x, 4);
    EXPECT_EQ(vector1.y, 6);
}

TEST(Vector2Tests, shouldSubtractAssignVectors)
{
    VectorUnderTest vector1(5, 6);
    VectorUnderTest vector2(3, 4);
    vector1 -= vector2;
    EXPECT_EQ(vector1.x, 2);
    EXPECT_EQ(vector1.y, 2);
}

TEST(Vector2Tests, shouldMultiplyAssignVectorByScalar)
{
    VectorUnderTest vector(2, 3);
    int scalar = 4;
    vector *= scalar;
    EXPECT_EQ(vector.x, 8);
    EXPECT_EQ(vector.y, 12);
}

TEST(Vector2Tests, shouldDivideAssignVectorByScalar)
{
    VectorUnderTest vector(8, 12);
    int scalar = 4;
    vector /= scalar;
    EXPECT_EQ(vector.x, 2);
    EXPECT_EQ(vector.y, 3);
}

TEST(Vector2Tests, shouldConstructFromAnotherVectorWithDifferentType)
{
    Vector2<float> vector1(1.5f, 2.5f);
    VectorUnderTest vector2(vector1);
    EXPECT_EQ(vector2.x, 1);
    EXPECT_EQ(vector2.y, 2);
}

}  // namespace engine
