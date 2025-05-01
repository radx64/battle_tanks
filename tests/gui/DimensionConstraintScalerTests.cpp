#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/DimensionConstraintScaler.hpp"

namespace gui
{

TEST(DimensionConstraintScalerShould, returnZeroForNoElements)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(0);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
}

TEST(DimensionConstraintScalerShould, returnWholeSizeForOneElementUnconstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(1);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 100.f);
}

TEST(DimensionConstraintScalerShould, returnZeroForOneZeroPixelsConstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(1);
    sut.setElementSize(0, SizeConstraint::Pixels(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
}

TEST(DimensionConstraintScalerShould, returnZeroForOneZeroPercentConstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(1);
    sut.setElementSize(0, SizeConstraint::Percent(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
}


TEST(DimensionConstraintScalerShould, calculateSizesForOneZeroPixelsConstrainedElementAndOneUnconstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(2);
    sut.setElementSize(0, SizeConstraint::Pixels(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 100.f);
}

TEST(DimensionConstraintScalerShould, calculateSizesForOneZeroPercentConstrainedElementAndOneUnconstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(2);
    sut.setElementSize(0, SizeConstraint::Percent(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 100.f);
}

TEST(DimensionConstraintScalerShould, calculateSizesForOneZeroPixelsConstrainedElementOneUnconstrainedElementAndOneZeroPercentConstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, SizeConstraint::Pixels(0.f));
    sut.setElementSize(2, SizeConstraint::Percent(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 100.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 0.f);
}

TEST(DimensionConstraintScalerShould, calculateSizesForOneZeroPercentConstrainedElementAndOneUnconstrainedElementAndOneZeroPixelsConstrainedElementAndOneUnconstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(4);
    sut.setElementSize(0, SizeConstraint::Percent(0.f));
    sut.setElementSize(2, SizeConstraint::Pixels(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 50.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(3), 50.f);
}

TEST(DimensionConstraintScalerShould, calculateSizesForAllConstrainedElementsOfMixedTypes)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, SizeConstraint::Pixels(20.f));
    sut.setElementSize(1, SizeConstraint::Percent(50.f));
    sut.setElementSize(2, SizeConstraint::Auto());
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 40.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 40.f);
}

TEST(DimensionConstraintScalerShould, calculateSizesForAllConstrainedAutoElementAndTwoPercentConstrainedElements)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, SizeConstraint::Auto());
    sut.setElementSize(1, SizeConstraint::Percent(25.f));
    sut.setElementSize(2, SizeConstraint::Percent(25.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 50.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 25.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 25.f);
}


TEST(DimensionConstraintScalerShould, calculateSizesForAllConstrainedElementsAndOneUnconstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(4);
    sut.setElementSize(0, SizeConstraint::Pixels(20.f));
    sut.setElementSize(1, SizeConstraint::Pixels(30.f));
    sut.setElementSize(2, SizeConstraint::Percent(50.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 30.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 25.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(3), 25.f);
}


TEST(DimensionConstraintScalerShould, clearPreviouslySetElementSize)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, SizeConstraint::Pixels(20.f));
    sut.setElementSize(1, SizeConstraint::Pixels(30.f));
    sut.setElementSize(2, SizeConstraint::Pixels(20.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 30.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 20.f);

    sut.resetElement(1);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 60.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 20.f);
}


TEST(DimensionConstraintScalerShould, addNewElementAtIndex)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, SizeConstraint::Pixels(10.f));
    sut.setElementSize(1, SizeConstraint::Pixels(20.f));
    sut.setElementSize(2, SizeConstraint::Pixels(30.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.addElementAtIndex(1, SizeConstraint::Auto());
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 40.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(3), 30.f);
}

TEST(DimensionConstraintScalerShould, removeElementAtIndex)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, SizeConstraint::Pixels(10.f));
    sut.setElementSize(1, SizeConstraint::Pixels(20.f));
    sut.setElementSize(2, SizeConstraint::Pixels(30.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.removeElementAtIndex(1);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 30.f);
}


TEST(DimensionConstraintScalerShould, handleInvalidIndex)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, SizeConstraint::Pixels(10.f));
    sut.setElementSize(1, SizeConstraint::Pixels(20.f));
    sut.setElementSize(2, SizeConstraint::Pixels(30.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.removeElementAtIndex(99);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.addElementAtIndex(99, SizeConstraint::Auto());
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.resetElement(99);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.setElementSize(99, SizeConstraint::Pixels(50.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    EXPECT_FLOAT_EQ(sut.getElementSize(99), 0.f);
}

TEST(DimensionConstraintScalerShould, notExceedSizeWhenCalculatingDimensionsForEachObjectWhenUsingPercentConstraints)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, SizeConstraint::Percent(40.f));
    sut.setElementSize(1, SizeConstraint::Percent(40.f));
    sut.setElementSize(2, SizeConstraint::Percent(50.f));

    auto sum  = sut.getElementSize(0) + sut.getElementSize(1) + sut.getElementSize(2);
    EXPECT_FLOAT_EQ(100.f, sum);

    sut.resetElement(1);

    sum  = sut.getElementSize(0) + sut.getElementSize(1) + sut.getElementSize(2);
    EXPECT_FLOAT_EQ(100.f, sum);

    sut.setElementCount(1);
    sut.setElementSize(0, SizeConstraint::Percent(200.f));

    sum  = sut.getElementSize(0);
    EXPECT_FLOAT_EQ(100.f, sum);
}

TEST(DimensionConstraintScalerShould, scaleElementsWhenResized)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementCount(2);
    sut.setElementSize(0, SizeConstraint::Auto());
    sut.setElementSize(1, SizeConstraint::Pixels(40.f));

    EXPECT_FLOAT_EQ(60.f, sut.getElementSize(0));
    EXPECT_FLOAT_EQ(40.f, sut.getElementSize(1));

    sut.setTotalSize(200.f);
    EXPECT_FLOAT_EQ(160.f, sut.getElementSize(0));
    EXPECT_FLOAT_EQ(40.f, sut.getElementSize(1));
}

} // namespace gui
