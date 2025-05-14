#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/layout/Horizontal.hpp"
#include "tests/mocks/ComponentSpy.hpp"

namespace gui::layout
{

using Spy = ::testing::NiceMock<mocks::ComponentSpy>;

TEST(HorizontalShould, createHorizontalLayout)
{
    auto sut = Horizontal::create();
    EXPECT_NE(sut, nullptr);
}

TEST(HorizontalShould, addChild)
{
    auto sut = Horizontal::create();
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    sut->addChild(std::move(child));
    EXPECT_EQ(sut->getChildren()[0]->getChildren()[0].get(), child_ptr);
}

TEST(HorizontalShould, addColumnIfPossible)
{
    auto sut = Horizontal::create();
    auto child = std::make_unique<Spy>();
    sut->addChild(std::move(child));
    EXPECT_TRUE(sut->addColumn(0, Constraint::Auto()));
    EXPECT_FALSE(sut->addColumn(10, Constraint::Auto()));
}

TEST(HorizontalShould, removeColumnIfPossible)
{
    auto sut = Horizontal::create();
    auto child = std::make_unique<Spy>();
    sut->addChild(std::move(child));
    sut->addColumn(0, Constraint::Auto());
    EXPECT_TRUE(sut->removeColumn(0));
    EXPECT_FALSE(sut->removeColumn(10));
}

TEST(HorizontalShould, setColumnSize)
{
    auto sut = Horizontal::create();
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();

    sut->addChild(std::move(child));
    sut->setSize({100, 100});
    sut->addColumn(0, Constraint::Auto());
    sut->setColumnSize(0, Constraint::Pixels(50));
    EXPECT_EQ(child_ptr->getSize().x, 50.f);
}

TEST(HorizontalShould, setPadding)
{
    auto sut = Horizontal::create();

    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    auto child2 = std::make_unique<Spy>();
    auto child2_ptr = child2.get();

    sut->setPadding(0);
    sut->addChild(std::move(child));
    sut->addChild(std::move(child2));
    sut->setColumnSize(0, Constraint::Pixels(20));
    sut->setColumnSize(1, Constraint::Auto());
    sut->setSize({100, 100});

    EXPECT_EQ(child_ptr->getSize().x, 20.f);
    EXPECT_EQ(child2_ptr->getSize().x, 80.f);

    sut->setPadding(10);
    sut->setSize({100, 100});

    EXPECT_EQ(child_ptr->getSize().x, 20.f);
    EXPECT_EQ(child2_ptr->getSize().x, 60.f);
}

TEST(HorizontalShould, removeChildIfColumnIsRemoved)
{
    auto sut = Horizontal::create();
    auto child = std::make_unique<Spy>();
    auto child_ptr = child.get();
    EXPECT_CALL(*child_ptr, die());
    sut->addChild(std::move(child));
    sut->removeColumn(0);
    ::testing::Mock::VerifyAndClearExpectations(child_ptr);
}

}  // namespace gui::layout
