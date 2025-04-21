#include <array>
#include <cmath>
#include <ostream>

#include <gtest/gtest.h>

#include "source/engine/math/Math.hpp"


namespace sf
{
// If you can't declare the function in the class it's important that PrintTo()
// is defined in the SAME namespace that defines Vector2f.  C++'s look-up rules
// rely on that.
void PrintTo(const Vector2f& point, std::ostream* os) 
{
    *os << "sf::Vector2f:(" << point.x << "," << point.y << ")";
}
}  // namespace sf

namespace engine::math
{

TEST(Distance, shouldReturnZeroForSinglePoint)
{
    float expected_distance = 0.0;
    float calculated_distance = engine::math::distance(2.0, 3.0, 2.0, 3.0);
    EXPECT_FLOAT_EQ(expected_distance, calculated_distance);
}

TEST(Distance, shouldReturnProperValues)
{
    float expected_distance = 1.4142135623730951;
    float calculated_distance = engine::math::distance(0.0, 0.0, 1.0, 1.0);
    EXPECT_FLOAT_EQ(expected_distance, calculated_distance);
}

TEST(Distance, shouldWorkForNegativeCoordinates)
{
    float expected_distance = 1.4142135623730951;
    float calculated_distance = engine::math::distance(0.0, 0.0, -1.0, -1.0);
    EXPECT_FLOAT_EQ(expected_distance, calculated_distance);
}

TEST(DotProduct, shouldCalculateForPositiveVectors)
{
    float expected_result = 5.f;
    float dotProduct = engine::math::dotProduct(1.0, 1.0, 2.0, 3.0);
    EXPECT_FLOAT_EQ(expected_result, dotProduct);
}

TEST(DotProduct, shouldCalculateForNegativeVectors)
{
    float expected_result = -9.f;
    float dotProduct = engine::math::dotProduct(1.0, 1.0, -2.0, -7.0);
    EXPECT_FLOAT_EQ(expected_result, dotProduct);
}

TEST(CrossProduct, shouldCalculateForPositiveVectors)
{
    float expected_result = 1.f;
    float crossProduct = engine::math::crossProduct(1.0, 1.0, 2.0, 3.0);
    EXPECT_FLOAT_EQ(expected_result, crossProduct);
}

TEST(CrossProduct, shouldCalculateForNegativeVectors)
{
    float expected_result = -5.f;
    float crossProduct = engine::math::crossProduct(1.0, 1.0, -2.0, -7.0);
    EXPECT_FLOAT_EQ(expected_result, crossProduct);
}

TEST(NormalizeVector, shouldReturnItsLengthBeforeNormalization)
{
    float expected_length = 2.f * std::sqrt(2.f);
    sf::Vector2f vector = {2.f, 2.f};
    float length =  engine::math::normalizeVector(vector);
    EXPECT_FLOAT_EQ(expected_length, length);
}

TEST(NormalizeVector, shouldNormalizeVectorWithPositiveValues)
{
    sf::Vector2f vector = {2.f, 2.f};
    sf::Vector2f expected_vector = {0.707107f, 0.707107f};
    engine::math::normalizeVector(vector);
    EXPECT_FLOAT_EQ(expected_vector.x, vector.x);
    EXPECT_FLOAT_EQ(expected_vector.y, vector.y);
}

TEST(NormalizeVector, shouldNormalizeVectorWithNegativeValues)
{
    sf::Vector2f vector = {2.f, -5.f};
    sf::Vector2f expected_vector = {0.37139067f, -0.92847669f};
    engine::math::normalizeVector(vector);
    EXPECT_FLOAT_EQ(expected_vector.x, vector.x);
    EXPECT_FLOAT_EQ(expected_vector.y, vector.y);
}

TEST(SignedFmod, shouldReturnRemainderOfDivisionWithProperSignForNegatives)
{
    float expected_result = -5.0;
    float calculated_result = engine::math::signedFmod(-25.0, -20.0);
    EXPECT_FLOAT_EQ(expected_result, calculated_result);
}

TEST(SignedFmod, shouldReturnRemainderOfDivisionWithProperSignForPositives)
{
    float expected_result = 5.0;
    float calculated_result = engine::math::signedFmod(25.0, 20.0);
    EXPECT_FLOAT_EQ(expected_result, calculated_result);
}

TEST(SignedFmod, shouldReturnNanIfSecondParameterApproachesTowardsZero)
{
    float calculated_result = engine::math::signedFmod(1337.0, 0.0);
    EXPECT_TRUE(std::isnan(calculated_result));    
}

TEST(DegreeToRadians, shouldConvertZeroDegree)
{
    float expected_result = 0.0;
    float calculated_result = engine::math::degreeToRadians(0.0);
    EXPECT_FLOAT_EQ(expected_result, calculated_result);   
}

TEST(DegreeToRadians, shouldConvertPositiveDegree)
{
    float expected_result = 2.0943951023931953;
    float calculated_result = engine::math::degreeToRadians(120.0);
    EXPECT_FLOAT_EQ(expected_result, calculated_result);   
}

TEST(DegreeToRadians, shouldConvertNegativeDegree)
{
    float expected_result = -0.52359877559829882;
    float calculated_result = engine::math::degreeToRadians(-30.0);
    EXPECT_FLOAT_EQ(expected_result, calculated_result);   
}

TEST(RotatePoint, shouldNotMoveIfPivotIsInSamePlaceAsPoint)
{
    sf::Vector2f point{1.0f, 1.0f};
    float angle{90.0};
    sf::Vector2f pivot{1.0f, 1.0f};

    sf::Vector2f expected_result{1.0f, 1.0f};
    auto result = engine::math::rotatePoint(point, angle, pivot);

    EXPECT_EQ(expected_result, result);
}

TEST(RotatePoint, shouldMovePointAroundPivotWithPositiveAngle)
{
    sf::Vector2f point{1.0f, 1.0f};
    float angle{90.0};
    sf::Vector2f pivot{0.0f, 0.0f};

    sf::Vector2f expected_result{-1.0f, 1.0f};
    auto result = engine::math::rotatePoint(point, angle, pivot);

    EXPECT_FLOAT_EQ(expected_result.x, result.x);
    EXPECT_FLOAT_EQ(expected_result.y, result.y);
}

TEST(RotatePoint, shouldMovePointAroundPivotWithNegativeAngle)
{
    sf::Vector2f point{1.0f, 1.0f};
    float angle{-90.0};
    sf::Vector2f pivot{0.0f, 0.0f};

    sf::Vector2f expected_result{1.0f, -1.0f};
    auto result = engine::math::rotatePoint(point, angle, pivot);

    EXPECT_FLOAT_EQ(expected_result.x, result.x);
    EXPECT_FLOAT_EQ(expected_result.y, result.y);
}

TEST(Average, shouldCalculateWindowAverageForFewValues)
{
    engine::math::Average average(4);
    int result{};
    for (const auto value : std::array<int, 4>{2,4,6,8})
    {
        result = average.calculate(value);
    }

    int expected_result = 5;

    EXPECT_EQ(expected_result, result);
}

TEST(Average, shouldCalculateWindowAverageForMoreValuesThanWindow)
{
    engine::math::Average average(4);
    int result{};
    for (const auto value : std::array<int, 6>{2,4,6,8,10,12})
    {
        result = average.calculate(value);
    }

    int expected_result = 9;

    EXPECT_EQ(expected_result, result);
}

TEST(Average, shouldThrowIfAverageWindowIsLessThen2)
{
    ASSERT_THROW(engine::math::Average(0), std::invalid_argument);
    ASSERT_THROW(engine::math::Average(1), std::invalid_argument);
    ASSERT_NO_THROW(engine::math::Average(2));
}

TEST(Clamp, shouldReturnNormalValueWhenNotOutOfBounds)
{
    float value = 5.0f;
    float min = 0.0f;
    float max = 10.0f;

    float expected_result = 5.0f;
    float result = engine::math::clamp(value, min, max);

    EXPECT_FLOAT_EQ(expected_result, result);
}

TEST(Clamp, shouldReturnMinimumValueWhenOutOfBounds)
{
    float value = -5.0f;
    float min = 0.0f;
    float max = 10.0f;

    float expected_result = 0.0f;
    float result = engine::math::clamp(value, min, max);

    EXPECT_FLOAT_EQ(expected_result, result);
}

TEST(Clamp, shouldReturnMaximumValueWhenOutOfBounds)
{
    float value = 15.0f;
    float min = 0.0f;
    float max = 10.0f;

    float expected_result = 10.0f;
    float result = engine::math::clamp(value, min, max);

    EXPECT_FLOAT_EQ(expected_result, result);
}

}  // namespace engine::math
