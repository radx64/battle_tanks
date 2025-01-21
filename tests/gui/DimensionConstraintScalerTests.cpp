#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/DimensionConstraintScaler.hpp"

namespace gui
{

TEST(DimensionConstraintScalerShoud, returnZeroForNoElements)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(0);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
}

TEST(DimensionConstraintScalerShoud, returnWholeSizeForOneElementUnconstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(1);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 100.f);
}

TEST(DimensionConstraintScalerShoud, returnZeroForOneZeroConstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(1);
    sut.setElementSize(0, 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
}

TEST(DimensionConstraintScalerShoud, calculateSizesForOneZeroConstrainedElementAndOneUnconstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(2);
    sut.setElementSize(0, 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 100.f);
}

TEST(DimensionConstraintScalerShoud, calculateSizesForOneZeroConstrainedElementAndOneUnconstrainedElementAndOneZeroConstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, 0.f);
    sut.setElementSize(2, 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 100.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 0.f);
}

TEST(DimensionConstraintScalerShoud,calculateSizesForOneZeroConstrainedElementAndOneUnconstrainedElementAndOneZeroConstrainedElementAndOneUnconstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(4);
    sut.setElementSize(0, 0.f);
    sut.setElementSize(2, 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 50.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(3), 50.f);
}

TEST(DimensionConstraintScalerShould, calculateSizesForAllConstrainedElements)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, 0.2f);
    sut.setElementSize(1, 0.3f);
    sut.setElementSize(2, 0.5f);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 30.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 50.f);
}

TEST(DimensionConstraintScalerShould, calculateSizesForAllConstrainedElementsAndOneUnconstrainedElement)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(4);
    sut.setElementSize(0, 0.2f);
    sut.setElementSize(1, 0.3f);
    sut.setElementSize(2, 0.4f);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 30.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 40.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(3), 10.f);
}

TEST(DimensionConstraintScalerShould, clearPreviouslySetElementSize)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, 0.2f);
    sut.setElementSize(1, 0.3f);
    sut.setElementSize(2, 0.2f);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 30.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 20.f);

    sut.clearElementSize(1);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 60.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 20.f);
}

TEST(DimensionConstraintScalerShould, addNewElementAtIndex)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, 0.1f);
    sut.setElementSize(1, 0.2f);
    sut.setElementSize(2, 0.3f);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.addElementAtIndex(1);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 40.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(3), 30.f);
}

TEST(DimensionConstraintScalerShould, removeElementAtIndex)
{
    DimensionConstraintScaler sut{"sut"};
    sut.setSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, 0.1f);
    sut.setElementSize(1, 0.2f);
    sut.setElementSize(2, 0.3f);
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
    sut.setSize(100.f);
    sut.setElementCount(3);
    sut.setElementSize(0, 0.1f);
    sut.setElementSize(1, 0.2f);
    sut.setElementSize(2, 0.3f);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.removeElementAtIndex(99);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.addElementAtIndex(99);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.clearElementSize(99);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.setElementSize(99, 0.5f);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    EXPECT_FLOAT_EQ(sut.getElementSize(99), 0.f);
}

} // namespace gui
