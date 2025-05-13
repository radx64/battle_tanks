#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/layout/Vertical.hpp"

namespace gui::layout
{

TEST(VerticalShould, createVerticalLayout)
{
    auto sut = Vertical::create();
    EXPECT_NE(sut, nullptr);
}

TEST(VerticalShould, addChild)
{
    auto sut = Vertical::create();
    auto child = std::make_unique<Base>();
    sut->addChild(std::move(child));
    EXPECT_EQ(sut->getChildren()[0]->getParent(), sut.get());
}

}  // namespace gui::layout
