#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/layout/Vertical.hpp"
#include "tests/mocks/ComponentSpy.hpp"

namespace gui::layout
{

using Spy = ::testing::NiceMock<mocks::ComponentSpy>;

TEST(VerticalShould, createVerticalLayout)
{
    auto sut = Vertical::create();
    EXPECT_NE(sut, nullptr);
}

TEST(VerticalShould, addChild)
{
    auto sut = Vertical::create();
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    sut->addChild(std::move(child));
    // Below double getChildren() seems a bit off but it is due the fact that Vertical has intermediate Grid object in between Vertical and child
    // and we need to get children of Grid as a children of a Vertical
    EXPECT_EQ(sut->getChildren()[0]->getChildren()[0].get(), child_ptr);
}

TEST(VerticalShould, addRowIfPossible)
{
    auto sut = Vertical::create();
    auto child = std::make_unique<Spy>();
    sut->addChild(std::move(child));
    EXPECT_TRUE(sut->addRow(0, Constraint::Auto()));
    EXPECT_FALSE(sut->addRow(10, Constraint::Auto()));
}

TEST(VerticalShould, removeRowIfPossible)
{
    auto sut = Vertical::create();
    auto child = std::make_unique<Spy>();
    sut->addChild(std::move(child));
    sut->addRow(0, Constraint::Auto());
    EXPECT_TRUE(sut->removeRow(0));
    EXPECT_FALSE(sut->removeRow(10));
}

TEST(VerticalShould, setRowSize)
{
    auto sut = Vertical::create();
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();

    sut->addChild(std::move(child));
    sut->setSize({100, 100});
    sut->addRow(0, Constraint::Auto());
    sut->setRowSize(0, Constraint::Pixels(50));
    EXPECT_EQ(child_ptr->getSize().y, 50.f);
}

TEST(VerticalShould, setPadding)
{
    auto sut = Vertical::create();

    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    auto child2 = std::make_unique<Spy>();
    auto child2_ptr = child2.get();

    sut->setPadding(0);
    sut->addChild(std::move(child));
    sut->addChild(std::move(child2));
    sut->setRowSize(0, Constraint::Pixels(20));
    sut->setRowSize(1, Constraint::Auto());
    sut->setSize({100, 100});

    EXPECT_EQ(child_ptr->getSize().y, 20.f);
    EXPECT_EQ(child2_ptr->getSize().y, 80.f);

    sut->setPadding(10);
    sut->setSize({100, 100});

    EXPECT_EQ(child_ptr->getSize().y, 20.f);
    EXPECT_EQ(child2_ptr->getSize().y, 60.f);
}

TEST(VerticalShould, removeChildIfRowIsRemoved)
{
    auto sut = Vertical::create();
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    EXPECT_CALL(*child_ptr, die());
    sut->addChild(std::move(child));
    sut->removeRow(0);
    ::testing::Mock::VerifyAndClearExpectations(child_ptr);
}

}  // namespace gui::layout
