#include <array>
#include <cmath>
#include <ostream>

#include <gtest/gtest.h>

#include "source/math/Math.hpp"

TEST(Distance, shouldReturnZeroForSinglePoint)
{
    float expected_distance = 0.0;
    float calculated_distance = math::distance(2.0, 3.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(expected_distance, calculated_distance);
}

TEST(Distance, shouldReturnProperValues)
{
    float expected_distance = 1.4142135623730951;
    float calculated_distance = math::distance(0.0, 0.0, 1.0, 1.0);
    EXPECT_DOUBLE_EQ(expected_distance, calculated_distance);
}

TEST(Distance, shouldWorkForNegativeCoordinates)
{
    float expected_distance = 1.4142135623730951;
    float calculated_distance = math::distance(0.0, 0.0, -1.0, -1.0);
    EXPECT_DOUBLE_EQ(expected_distance, calculated_distance);
}

TEST(SignedFmod, shouldReturnRemainderOfDivisionWithProperSignForNegatives)
{
    float expected_result = -5.0;
    float calculated_result = math::signed_fmod(-25.0, -20.0);
    EXPECT_DOUBLE_EQ(expected_result, calculated_result);
}

TEST(SignedFmod, shouldReturnRemainderOfDivisionWithProperSignForPositives)
{
    float expected_result = 5.0;
    float calculated_result = math::signed_fmod(25.0, 20.0);
    EXPECT_DOUBLE_EQ(expected_result, calculated_result);
}

TEST(SignedFmod, shouldReturnNanIfSecondParameterApproachesTowardsZero)
{
    float calculated_result = math::signed_fmod(1337.0, 0.0);
    EXPECT_TRUE(std::isnan(calculated_result));    
}

TEST(DegeeToRadians, shouldConvertZeroDegree)
{
    float expected_result = 0.0;
    float calculated_result = math::degree_to_radians(0.0);
    EXPECT_DOUBLE_EQ(expected_result, calculated_result);   
}

TEST(DegeeToRadians, shouldConvertPositiveDegree)
{
    float expected_result = 2.0943951023931953;
    float calculated_result = math::degree_to_radians(120.0);
    EXPECT_DOUBLE_EQ(expected_result, calculated_result);   
}

TEST(DegeeToRadians, shouldConvertNegativeDegree)
{
    float expected_result = -0.52359877559829882;
    float calculated_result = math::degree_to_radians(-30.0);
    EXPECT_DOUBLE_EQ(expected_result, calculated_result);   
}

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

TEST(RotatePoint, shouldNotMoveIfPivotIsInSamePlaceAsPoint)
{
    sf::Vector2f point{1.0f, 1.0f};
    float angle{90.0};
    sf::Vector2f pivot{1.0f, 1.0f};

    sf::Vector2f expected_result{1.0f, 1.0f};
    auto result = math::rotate_point(point, angle, pivot);

    EXPECT_EQ(expected_result, result);
}

TEST(RotatePoint, shouldMovePointAroundPivotWithPositiveAngle)
{
    sf::Vector2f point{1.0f, 1.0f};
    float angle{90.0};
    sf::Vector2f pivot{0.0f, 0.0f};

    sf::Vector2f expected_result{-1.0f, 1.0f};
    auto result = math::rotate_point(point, angle, pivot);

    EXPECT_FLOAT_EQ(expected_result.x, result.x);
    EXPECT_FLOAT_EQ(expected_result.y, result.y);
}

TEST(RotatePoint, shouldMovePointAroundPivotWithNegativeAngle)
{
    sf::Vector2f point{1.0f, 1.0f};
    float angle{-90.0};
    sf::Vector2f pivot{0.0f, 0.0f};

    sf::Vector2f expected_result{1.0f, -1.0f};
    auto result = math::rotate_point(point, angle, pivot);

    EXPECT_FLOAT_EQ(expected_result.x, result.x);
    EXPECT_FLOAT_EQ(expected_result.y, result.y);
}

TEST(Average, shouldCalculateWindowAverageForFewValues)
{
    math::Average average(4);
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
    math::Average average(4);
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
    ASSERT_THROW(math::Average(0), std::invalid_argument);
    ASSERT_THROW(math::Average(1), std::invalid_argument);
    ASSERT_NO_THROW(math::Average(2));
}