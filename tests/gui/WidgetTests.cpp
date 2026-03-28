#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <SFML/Graphics.hpp>

#include "gui/Widget.hpp"

#include "tests/mocks/WidgetSpy.hpp"

namespace gui
{

using WidgetSpy = mocks::WidgetSpy;

/***************************
     POINTERS MGMT TESTS
****************************/

TEST(WidgetShould, handleChildObjectsDestructionWhenDestroyed)
{
    auto sut_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_1 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_2 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_3 = std::make_unique<::testing::NiceMock<WidgetSpy>>();

    auto child_1_ptr = child_1.get();
    auto child_2_ptr = child_2.get();
    auto child_3_ptr = child_3.get();

    sut_->addChild(std::move(child_1));
    sut_->addChild(std::move(child_2));

    EXPECT_CALL(*child_1_ptr, die()).Times(1);
    EXPECT_CALL(*child_2_ptr, die()).Times(1);
    EXPECT_CALL(*child_3_ptr, die()).Times(0);
    sut_.reset();
    // Destructor of test body will destroy mocks nevertheless
    // so it is important to check before test body ends
    ::testing::Mock::VerifyAndClearExpectations(child_1_ptr);
    ::testing::Mock::VerifyAndClearExpectations(child_2_ptr);
    ::testing::Mock::VerifyAndClearExpectations(child_3_ptr);
    sut_.reset();
    child_1.reset();
    child_2.reset();
    child_3.reset();

    EXPECT_TRUE(::testing::Mock::VerifyAndClearExpectations(sut_.get()));
    sut_.reset();
}

TEST(WidgetShould, beAbleToRemoveChildrenOnDemand)
{
    auto sut_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_1 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_2 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_3 = std::make_unique<::testing::NiceMock<WidgetSpy>>();

    auto child_1_ptr = child_1.get();
    auto child_2_ptr = child_2.get();
    auto child_3_ptr = child_3.get();

    sut_->addChild(std::move(child_1));
    sut_->addChild(std::move(child_2));
    sut_->addChild(std::move(child_3));

    EXPECT_CALL(*child_1_ptr, die()).Times(1);
    EXPECT_CALL(*child_2_ptr, die()).Times(1);
    EXPECT_CALL(*child_3_ptr, die()).Times(0);

    sut_->removeChild(child_1_ptr);
    sut_->removeChild(child_2_ptr);
    sut_->removeChild(nullptr);

    // Destructor of test body will destroy mocks nevertheless
    // so it is important to check before test body ends
    ::testing::Mock::VerifyAndClearExpectations(child_1_ptr);
    ::testing::Mock::VerifyAndClearExpectations(child_2_ptr);
    ::testing::Mock::VerifyAndClearExpectations(child_3_ptr);
}

TEST(WidgetShould, handleChildObjectsDestructionWhenDestroyedEvenInHierarchy)
{
    auto sut_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_1 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_2 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_3 = std::make_unique<::testing::NiceMock<WidgetSpy>>();

    auto child_1_ptr = child_1.get();
    auto child_2_ptr = child_2.get();
    auto child_3_ptr = child_3.get();

    child_2->addChild(std::move(child_3));
    child_1->addChild(std::move(child_2));
    sut_->addChild(std::move(child_1));

    EXPECT_CALL(*child_1_ptr, die()).Times(1);
    EXPECT_CALL(*child_2_ptr, die()).Times(1);
    EXPECT_CALL(*child_3_ptr, die()).Times(1);
    sut_.reset();
    // Destructor of test body will destroy mocks nevertheless
    // so it is important to check before test body ends
    ::testing::Mock::VerifyAndClearExpectations(child_1_ptr);
    ::testing::Mock::VerifyAndClearExpectations(child_2_ptr);
    ::testing::Mock::VerifyAndClearExpectations(child_3_ptr);
}

// This test is disabled until i will figure out how to properly mock rendering
// in X11 headless environment like github actions
TEST(WidgetShould, callOnRenderMethodOnlyWhenWidgetIsVisible)
{
    auto sut_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto sut_ptr = sut_.get();

    sf::RenderTexture guiRenderTexture;

    EXPECT_CALL(*sut_ptr, render_mock()).Times(1);
    sut_->setVisibility(true);
    sut_->render(guiRenderTexture);

    EXPECT_CALL(*sut_ptr, render_mock()).Times(0);
    sut_->setVisibility(false);
    sut_->render(guiRenderTexture);
}

/*********************
     DRAWING TESTS
**********************/
TEST(WidgetShould, handleItsVisibilityParameter)
{
    auto sut_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    sut_->setVisibility(false);

    EXPECT_FALSE(sut_->isVisible());

    sut_->setVisibility(true);
    EXPECT_TRUE(sut_->isVisible());
}

// This test is disabled until i will figure out how to properly mock rendering
// in X11 headless environment like github actions
TEST(WidgetShould, renderChildrenOnlyWhenWidgetItselfIsVisible)
{
    auto sut_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_1 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto child_2 = std::make_unique<::testing::NiceMock<WidgetSpy>>();

    auto sut_ptr = sut_.get();
    auto child_1_ptr = child_1.get();
    auto child_2_ptr = child_2.get();

    sut_->addChild(std::move(child_1));
    sut_->addChild(std::move(child_2));

    sf::RenderTexture guiRenderTexture;
    EXPECT_CALL(*sut_ptr, render_mock()).Times(1);
    EXPECT_CALL(*child_1_ptr, render_mock()).Times(1);
    EXPECT_CALL(*child_2_ptr, render_mock()).Times(1);
    sut_->setVisibility(true);
    sut_->render(guiRenderTexture);

    EXPECT_CALL(*sut_ptr, render_mock()).Times(0);
    EXPECT_CALL(*child_1_ptr, render_mock()).Times(0);
    EXPECT_CALL(*child_2_ptr, render_mock()).Times(0);
    sut_->setVisibility(false);
    sut_->render(guiRenderTexture);
}

/*******************************
      BOUNDS HANDLING TESTS
********************************/
TEST(WidgetShould, setAndGetItsSize)
{
    auto sut_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto required_size = sf::Vector2f{2.f, 9.f};
    sut_->setSize(required_size);

    EXPECT_EQ(sut_->getSize(), required_size);
}

TEST(WidgetShould, KnowOwnPositionWithoutParent)
{
    auto sut_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();

    sut_->setPosition(sf::Vector2f{10.f,20.f});

    auto expected_position = sf::Vector2f{10.f, 20.f};
    EXPECT_EQ(sut_->getPosition(), expected_position);
    EXPECT_EQ(sut_->getGlobalPosition(), expected_position);
}

TEST(WidgetShould, KnowOwnPositionWithParent)
{
    auto sut_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    auto parent_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();

    parent_->setPosition(sf::Vector2f{10.f,20.f});
    sut_->setPosition(sf::Vector2f{20.f,10.f});
    auto sut_ptr = sut_.get();
    parent_->addChild(std::move(sut_));

    auto expected_global_position = sf::Vector2f{30.f, 30.f};
    auto expected_local_position = sf::Vector2f{20.f, 10.f};
    EXPECT_EQ(sut_ptr->getPosition(), expected_local_position);
    EXPECT_EQ(sut_ptr->getGlobalPosition(), expected_global_position);
}

TEST(WidgetShould, DetectIfPointIsInsideOfWidget)
{
    auto sut_ = std::make_unique<::testing::NiceMock<WidgetSpy>>();
    sut_->setPosition(sf::Vector2f{20.f,10.f});
    sut_->setSize(sf::Vector2f{10.f,10.f});

    EXPECT_TRUE(sut_->isInside(sf::Vector2f{25.f, 15.f}));
    EXPECT_FALSE(sut_->isInside(sf::Vector2f{0.f, 5.f}));
}

}  // namespace gui
