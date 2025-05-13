#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/layout/SizeConstraintResolver.hpp"

namespace gui::layout
{

TEST(SizeConstraintResolverShould, returnZeroForNoElements)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(0);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
}

TEST(SizeConstraintResolverShould, returnWholeSizeForOneElementUnconstrainedElement)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(1);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 100.f);
}

TEST(SizeConstraintResolverShould, returnZeroForOneZeroPixelsConstrainedElement)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(1);
    sut.setElementSize(0, Constraint::Pixels(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
}

TEST(SizeConstraintResolverShould, returnZeroForOneZeroPercentConstrainedElement)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(1);
    sut.setElementSize(0, Constraint::Percent(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
}


TEST(SizeConstraintResolverShould, calculateSizesForOneZeroPixelsConstrainedElementAndOneUnconstrainedElement)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(2);
    sut.setElementSize(0, Constraint::Pixels(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 100.f);
}

TEST(SizeConstraintResolverShould, calculateSizesForOneZeroPercentConstrainedElementAndOneUnconstrainedElement)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(2);
    sut.setElementSize(0, Constraint::Percent(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 100.f);
}

TEST(SizeConstraintResolverShould, calculateSizesForOneZeroPixelsConstrainedElementOneUnconstrainedElementAndOneZeroPercentConstrainedElement)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(3);
    sut.setElementSize(0, Constraint::Pixels(0.f));
    sut.setElementSize(2, Constraint::Percent(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 100.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 0.f);
}

TEST(SizeConstraintResolverShould, calculateSizesForOneZeroPercentConstrainedElementAndOneUnconstrainedElementAndOneZeroPixelsConstrainedElementAndOneUnconstrainedElement)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(4);
    sut.setElementSize(0, Constraint::Percent(0.f));
    sut.setElementSize(2, Constraint::Pixels(0.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 50.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 0.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(3), 50.f);
}

TEST(SizeConstraintResolverShould, calculateSizesForAllConstrainedElementsOfMixedTypes)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(3);
    sut.setElementSize(0, Constraint::Pixels(20.f));
    sut.setElementSize(1, Constraint::Percent(50.f));
    sut.setElementSize(2, Constraint::Auto());
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 40.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 40.f);
}

TEST(SizeConstraintResolverShould, calculateSizesForAllConstrainedAutoElementAndTwoPercentConstrainedElements)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(3);
    sut.setElementSize(0, Constraint::Auto());
    sut.setElementSize(1, Constraint::Percent(25.f));
    sut.setElementSize(2, Constraint::Percent(25.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 50.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 25.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 25.f);
}


TEST(SizeConstraintResolverShould, calculateSizesForAllConstrainedElementsAndOneUnconstrainedElement)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(4);
    sut.setElementSize(0, Constraint::Pixels(20.f));
    sut.setElementSize(1, Constraint::Pixels(30.f));
    sut.setElementSize(2, Constraint::Percent(50.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 30.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 25.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(3), 25.f);
}


TEST(SizeConstraintResolverShould, clearPreviouslySetElementSize)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(3);
    sut.setElementSize(0, Constraint::Pixels(20.f));
    sut.setElementSize(1, Constraint::Pixels(30.f));
    sut.setElementSize(2, Constraint::Pixels(20.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 30.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 20.f);

    sut.resetElement(1);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 60.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 20.f);
}


TEST(SizeConstraintResolverShould, addNewElementAtIndex)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(3);
    sut.setElementSize(0, Constraint::Pixels(10.f));
    sut.setElementSize(1, Constraint::Pixels(20.f));
    sut.setElementSize(2, Constraint::Pixels(30.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.addElementAtIndex(1, Constraint::Auto());
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 40.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(3), 30.f);
}

TEST(SizeConstraintResolverShould, removeElementAtIndex)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(3);
    sut.setElementSize(0, Constraint::Pixels(10.f));
    sut.setElementSize(1, Constraint::Pixels(20.f));
    sut.setElementSize(2, Constraint::Pixels(30.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.removeElementAtIndex(1);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 30.f);
}


TEST(SizeConstraintResolverShould, handleInvalidIndex)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(3);
    sut.setElementSize(0, Constraint::Pixels(10.f));
    sut.setElementSize(1, Constraint::Pixels(20.f));
    sut.setElementSize(2, Constraint::Pixels(30.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.removeElementAtIndex(99);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.addElementAtIndex(99, Constraint::Auto());
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.resetElement(99);
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    sut.setElementSize(99, Constraint::Pixels(50.f));
    EXPECT_FLOAT_EQ(sut.getElementSize(0), 10.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(1), 20.f);
    EXPECT_FLOAT_EQ(sut.getElementSize(2), 30.f);

    EXPECT_FLOAT_EQ(sut.getElementSize(99), 0.f);
}

TEST(SizeConstraintResolverShould, notExceedSizeWhenCalculatingDimensionsForEachObjectWhenUsingPercentConstraints)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(3);
    sut.setElementSize(0, Constraint::Percent(40.f));
    sut.setElementSize(1, Constraint::Percent(40.f));
    sut.setElementSize(2, Constraint::Percent(50.f));

    auto sum  = sut.getElementSize(0) + sut.getElementSize(1) + sut.getElementSize(2);
    EXPECT_FLOAT_EQ(100.f, sum);

    sut.resetElement(1);

    sum  = sut.getElementSize(0) + sut.getElementSize(1) + sut.getElementSize(2);
    EXPECT_FLOAT_EQ(100.f, sum);

    sut.setElementsCount(1);
    sut.setElementSize(0, Constraint::Percent(200.f));

    sum  = sut.getElementSize(0);
    EXPECT_FLOAT_EQ(100.f, sum);
}

TEST(SizeConstraintResolverShould, scaleElementsWhenResized)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(2);
    sut.setElementSize(0, Constraint::Auto());
    sut.setElementSize(1, Constraint::Pixels(40.f));

    EXPECT_FLOAT_EQ(60.f, sut.getElementSize(0));
    EXPECT_FLOAT_EQ(40.f, sut.getElementSize(1));

    sut.setTotalSize(200.f);
    EXPECT_FLOAT_EQ(160.f, sut.getElementSize(0));
    EXPECT_FLOAT_EQ(40.f, sut.getElementSize(1));
}

TEST(SizeConstraintResolverShould, returnElementCount)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(3);
    EXPECT_EQ(3, sut.getElementsCount());
    sut.setElementsCount(5);
    EXPECT_EQ(5, sut.getElementsCount());
    sut.setElementsCount(0);
    EXPECT_EQ(0, sut.getElementsCount());
}

TEST(SizeConstraintResolverShould, clampValueToZeroIfNegative)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(1);
    sut.setElementSize(0, Constraint::Pixels(-10.f));
    EXPECT_FLOAT_EQ(0.f, sut.getElementSize(0));
}

TEST(SizeConstraintResolverShould, precentsDoesNotWorkWhenWholeSizeIsUsedByFixedValues)
{
    SizeConstraintResolver sut{"sut"};
    sut.setTotalSize(100.f);
    sut.setElementsCount(2);
    sut.setElementSize(0, Constraint::Pixels(200.f));
    sut.setElementSize(1, Constraint::Percent(50.f));
    EXPECT_FLOAT_EQ(200.f, sut.getElementSize(0));
    EXPECT_FLOAT_EQ(0.f, sut.getElementSize(1));
}

} // namespace gui::layout
