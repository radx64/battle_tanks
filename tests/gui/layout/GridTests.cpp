#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/layout/Grid.hpp"
#include "tests/mocks/ComponentSpy.hpp"

namespace gui::layout
{

using Spy = ::testing::NiceMock<mocks::ComponentSpy>;

TEST(GridShould, createGridLayout)
{
    auto sut = Grid::create(2, 2);
    EXPECT_NE(sut, nullptr);
}

// As Vertical and Horizontal are now using Grid as a base class
// this test is not needed anymore so I'm testing only grid specific parts here

TEST(GridShould, addChild)
{
    auto sut = Grid::create(2, 2);
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    sut->addChild(std::move(child));
    EXPECT_EQ(sut->getChildren()[0].get(), child_ptr);
}

TEST(GridShould, setAndGetElementAt)
{
    auto sut = Grid::create(2, 2);
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    auto child2 = std::make_unique<Spy>();
    auto child2_ptr = child2.get();
    sut->setElementAt(0, 0, std::move(child));
    EXPECT_EQ(sut->getElementAt(0, 0), child_ptr);
    sut->setElementAt(0, 1, std::move(child2));
    EXPECT_EQ(sut->getElementAt(0, 1), child2_ptr);
    EXPECT_EQ(sut->getElementAt(1, 0), nullptr);
    EXPECT_EQ(sut->getElementAt(1, 1), nullptr);
}

TEST(GridShould, notSetOrGetElementAtIfOutOfBounds)
{
    auto sut = Grid::create(2, 2);
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    sut->setElementAt(0, 0, std::move(child));
    EXPECT_EQ(sut->getElementAt(0, 0), child_ptr);
    sut->setElementAt(10, 10, std::move(child));
    EXPECT_EQ(sut->getElementAt(10, 10), nullptr);
}

TEST(GridShould, replaceElementAt)
{
    auto sut = Grid::create(2, 2);
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    auto child2 = std::make_unique<Spy>();
    auto child2_ptr = child2.get();
    sut->setElementAt(0, 0, std::move(child));
    EXPECT_EQ(sut->getElementAt(0, 0), child_ptr);
    EXPECT_CALL(*child_ptr, die());
    sut->setElementAt(0, 0, std::move(child2));
    EXPECT_EQ(sut->getElementAt(0, 0), child2_ptr);
    EXPECT_EQ(sut->getElementAt(0, 1), nullptr);
    EXPECT_EQ(sut->getElementAt(1, 0), nullptr);
    EXPECT_EQ(sut->getElementAt(1, 1), nullptr);
    ::testing::Mock::VerifyAndClearExpectations(child_ptr);
}

TEST(GridShould, replaceElementAtWithNullptr)
{
    auto sut = Grid::create(2, 2);
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    sut->setElementAt(0, 0, std::move(child));
    EXPECT_EQ(sut->getElementAt(0, 0), child_ptr);
    EXPECT_CALL(*child_ptr, die());
    sut->setElementAt(0, 0, nullptr);
    EXPECT_EQ(sut->getElementAt(0, 0), nullptr);
    ::testing::Mock::VerifyAndClearExpectations(child_ptr);
}

TEST(GridShould, removeElementAt)
{
    auto sut = Grid::create(2, 2);
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    sut->setElementAt(0, 0, std::move(child));
    EXPECT_EQ(sut->getElementAt(0, 0), child_ptr);
    EXPECT_CALL(*child_ptr, die());
    sut->removeElementAt(0, 0);
    EXPECT_EQ(sut->getElementAt(0, 0), nullptr);
    ::testing::Mock::VerifyAndClearExpectations(child_ptr);
}

TEST(GridShould, doNotAddChildIfGridIsFull)
{
    auto sut = Grid::create(1, 1);
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    sut->setElementAt(0, 0, std::move(child));
    EXPECT_EQ(sut->getElementAt(0, 0), child_ptr);
    auto child2 = std::make_unique<Spy>();
    auto child2_ptr = child2.get();
    EXPECT_CALL(*child_ptr, die()).Times(0);
    EXPECT_CALL(*child2_ptr, die()).Times(1);
    sut->addChild(std::move(child2));
    ::testing::Mock::VerifyAndClearExpectations(child_ptr);
    ::testing::Mock::VerifyAndClearExpectations(child2_ptr);
}

}  // namespace gui::layout
