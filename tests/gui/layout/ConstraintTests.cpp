#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include "gui/layout/Constraint.hpp"

namespace gui::layout
{

TEST(ConstraintShould, createAutoConstraint)
{
    auto sut = Constraint::Auto();
    EXPECT_EQ(sut.getType(), Constraint::Type::Auto);
    EXPECT_EQ(sut.getUnit(), Constraint::Unit::Percentage);
    EXPECT_FLOAT_EQ(sut.getValue(), 0.f);
}

TEST(ConstraintShould, createPixelsConstraint)
{
    auto sut = Constraint::Pixels(10.f);
    EXPECT_EQ(sut.getType(), Constraint::Type::Fixed);
    EXPECT_EQ(sut.getUnit(), Constraint::Unit::Pixels);
    EXPECT_FLOAT_EQ(sut.getValue(), 10.f);
}

TEST(ConstraintShould, createPercentConstraint)
{
    auto sut = Constraint::Percent(10.f);
    EXPECT_EQ(sut.getType(), Constraint::Type::Fixed);
    EXPECT_EQ(sut.getUnit(), Constraint::Unit::Percentage);
    EXPECT_FLOAT_EQ(sut.getValue(), 10.f);
}

TEST(ConstraintShould, setType)
{
    auto sut = Constraint::Auto();
    sut.setType(Constraint::Type::Fixed);
    EXPECT_EQ(sut.getType(), Constraint::Type::Fixed);
    sut.setType(Constraint::Type::Auto);
    EXPECT_EQ(sut.getType(), Constraint::Type::Auto);
}

TEST(ConstraintShould, setUnit)
{
    auto sut = Constraint::Auto();
    sut.setUnit(Constraint::Unit::Pixels);
    EXPECT_EQ(sut.getUnit(), Constraint::Unit::Pixels);
    sut.setUnit(Constraint::Unit::Percentage);
    EXPECT_EQ(sut.getUnit(), Constraint::Unit::Percentage);
}

TEST(ConstraintShould, setValue)
{
    auto sut = Constraint::Auto();
    sut.setValue(10.f);
    EXPECT_FLOAT_EQ(sut.getValue(), 10.f);
    sut.setValue(20.f);
    EXPECT_FLOAT_EQ(sut.getValue(), 20.f);
}

}  // namespace gui::layout
