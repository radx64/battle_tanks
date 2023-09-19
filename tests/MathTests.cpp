#include <array>
#include <cmath>
#include <ostream>

#include <gtest/gtest.h>

#include "source/Math.hpp"

TEST(Distance, shouldReturnZeroForSinglePoint)
{
    double expected_distance = 0.0;
    double calculated_distance = math::distance(2.0, 3.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(expected_distance, calculated_distance);
}

TEST(Distance, shouldReturnProperValues)
{
    double expected_distance = 1.4142135623730951;
    double calculated_distance = math::distance(0.0, 0.0, 1.0, 1.0);
    EXPECT_DOUBLE_EQ(expected_distance, calculated_distance);
}

TEST(Distance, shouldWorkForNegativeCoordinates)
{
    double expected_distance = 1.4142135623730951;
    double calculated_distance = math::distance(0.0, 0.0, -1.0, -1.0);
    EXPECT_DOUBLE_EQ(expected_distance, calculated_distance);
}

TEST(SignedFmod, shouldReturnRemainderOfDivisionWithProperSignForNegatives)
{
    double expected_result = -5.0;
    double calculated_result = math::signed_fmod(-25.0, -20.0);
    EXPECT_DOUBLE_EQ(expected_result, calculated_result);
}

TEST(SignedFmod, shouldReturnRemainderOfDivisionWithProperSignForPositives)
{
    double expected_result = 5.0;
    double calculated_result = math::signed_fmod(25.0, 20.0);
    EXPECT_DOUBLE_EQ(expected_result, calculated_result);
}

TEST(SignedFmod, shouldReturnNanIfSecondParameterApproachesTowardsZero)
{
    double calculated_result = math::signed_fmod(1337.0, 0.0);
    EXPECT_TRUE(std::isnan(calculated_result));    
}

TEST(DegeeToRadians, shouldConvertZeroDegree)
{
    double expected_result = 0.0;
    double calculated_result = math::degree_to_radians(0.0);
    EXPECT_DOUBLE_EQ(expected_result, calculated_result);   
}

TEST(DegeeToRadians, shouldConvertPositiveDegree)
{
    double expected_result = 2.0943951023931953;
    double calculated_result = math::degree_to_radians(120.0);
    EXPECT_DOUBLE_EQ(expected_result, calculated_result);   
}

TEST(DegeeToRadians, shouldConvertNegativeDegree)
{
    double expected_result = -0.52359877559829882;
    double calculated_result = math::degree_to_radians(-30.0);
    EXPECT_DOUBLE_EQ(expected_result, calculated_result);   
}

std::ostream& operator<<(std::ostream& os,  const sf::Vector2f& vector)
{
    os <<"sf::Vector2f: ["<< vector.x <<", "<< vector.y << "]";
    return os;
}

TEST(RotatePoint, shouldNotMoveIfPivotIsInSamePlaceAsPoint)
{
    sf::Vector2f point{1.0f, 1.0f};
    double angle{90.0};
    sf::Vector2f pivot{1.0f, 1.0f};

    sf::Vector2f expected_result{1.0f, 1.0f};
    auto result = math::rotate_point(point, angle, pivot);

    EXPECT_EQ(expected_result, result);
}

TEST(RotatePoint, shouldMovePointAroundPivotWithPositiveAngle)
{
    sf::Vector2f point{1.0f, 1.0f};
    double angle{90.0};
    sf::Vector2f pivot{0.0f, 0.0f};

    sf::Vector2f expected_result{-1.0f, 1.0f};
    auto result = math::rotate_point(point, angle, pivot);

    EXPECT_EQ(expected_result, result);
}

TEST(RotatePoint, shouldMovePointAroundPivotWithNegativeAngle)
{
    sf::Vector2f point{1.0f, 1.0f};
    double angle{-90.0};
    sf::Vector2f pivot{0.0f, 0.0f};

    sf::Vector2f expected_result{1.0f, -1.0f};
    auto result = math::rotate_point(point, angle, pivot);

    EXPECT_EQ(expected_result, result);
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