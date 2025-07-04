#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

#include "tests/mocks/ComponentSpy.hpp"

#include "gui/debug/ComponentTree.hpp"

namespace gui
{

using ComponentSpy = mocks::ComponentSpy;

/***************************
     POINTERS MGMT TESTS
****************************/

TEST(ComponentShould, handleChildObjectsDestructionWhenDestroyed)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_3 = std::make_unique<::testing::NiceMock<ComponentSpy>>();

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

TEST(ComponentShould, beAbleToRemoveChildrenOnDemand)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_3 = std::make_unique<::testing::NiceMock<ComponentSpy>>();

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

TEST(ComponentShould, handleChildObjectsDestructionWhenDestroyedEvenInHierarchy)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_3 = std::make_unique<::testing::NiceMock<ComponentSpy>>();

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
TEST(ComponentShould, DISABLED_callOnRenderMethodOnlyWhenComponentIsVisible)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
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
TEST(ComponentShould, handleItsVisibilityParameter)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    sut_->setVisibility(false);

    EXPECT_FALSE(sut_->isVisible());

    sut_->setVisibility(true);
    EXPECT_TRUE(sut_->isVisible());
}

// This test is disabled until i will figure out how to properly mock rendering
// in X11 headless environment like github actions
TEST(ComponentShould, DISABLED_renderChildrenOnlyWhenComponentItselfIsVisible)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();

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
      EVENTS HANDLING TESTS
********************************/

class ComponentsHierarchyShould : public ::testing::Test
{
/*
    Components hierarchy: (all focusable)
    sut
        child_1
            child_1_1
            child_1_2
        child_2
            child_2_1
            child_2_2
            child_2_3
        child_3
*/

protected:

    void buildComponentsTree()
    {
        sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_1_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_1_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_2_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_2_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_2_3 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_3 = std::make_unique<::testing::NiceMock<ComponentSpy>>();

        child_1_ptr_ = child_1.get();
        child_1_2_ptr_ = child_1_2.get();
        child_1_1_ptr_ = child_1_1.get();
        child_2_ptr_ = child_2.get();
        child_2_1_ptr_ = child_2_1.get();
        child_2_2_ptr_ = child_2_2.get();
        child_2_3_ptr_ = child_2_3.get();
        child_3_ptr_ = child_3.get();

        allSpies_ = {
            sut_.get(),
            child_1_ptr_,
            child_1_1_ptr_,
            child_1_2_ptr_,
            child_2_ptr_,
            child_2_1_ptr_,
            child_2_2_ptr_,
            child_2_3_ptr_,
            child_3_ptr_,
        };

        sut_->addChild(std::move(child_1));
        sut_->addChild(std::move(child_2));
        sut_->addChild(std::move(child_3));

        child_1_ptr_->addChild(std::move(child_1_1));
        child_1_ptr_->addChild(std::move(child_1_2));

        child_2_ptr_->addChild(std::move(child_2_1));
        child_2_ptr_->addChild(std::move(child_2_2));
        child_2_ptr_->addChild(std::move(child_2_3));

    }

    void expectFocusGainedOnlyOn(ComponentSpy* expected)
    {
        for (auto* spy : allSpies_)
        {
            if (spy == expected)
            {
                EXPECT_CALL(*spy, focusGained()).Times(1);
            }
            else
            {
                EXPECT_CALL(*spy, focusGained()).Times(0);
            }
        }
    }

    void expectFocusLostOnlyOn(ComponentSpy* expected)
    {
        for (auto* spy : allSpies_)
        {
            if (spy == expected)
            {
                EXPECT_CALL(*spy, focusLost()).Times(1);
            }
            else
            {
                EXPECT_CALL(*spy, focusLost()).Times(0);
            }
        }
    }

    void verifyAndClearAllMocks()
    {
        for (auto* spy : allSpies_)
        {
            ::testing::Mock::VerifyAndClearExpectations(spy);
        }
    };

    std::unique_ptr<ComponentSpy> sut_;
    ComponentSpy* child_1_ptr_;
    ComponentSpy* child_1_2_ptr_;
    ComponentSpy* child_1_1_ptr_;
    ComponentSpy* child_2_ptr_;
    ComponentSpy* child_2_1_ptr_;
    ComponentSpy* child_2_2_ptr_;
    ComponentSpy* child_2_3_ptr_;
    ComponentSpy* child_3_ptr_;

    std::vector<ComponentSpy*> allSpies_;
};

TEST_F(ComponentsHierarchyShould, properlyHandleForwardFocusChangedEvent)
{
    buildComponentsTree();

    for (auto* spy : allSpies_)
    {
        spy->makeSpyFocusable();
    }

    for (size_t index=0; index < allSpies_.size(); ++index)
    {
        expectFocusGainedOnlyOn(allSpies_[index]);
        if (index > 0)
        {
            expectFocusLostOnlyOn(allSpies_[index-1]);
        }
        sut_->receive(gui::event::FocusChange{.type=gui::event::FocusChange::Type::Next});
        verifyAndClearAllMocks();
    }

    debug::ComponentTree::print(sut_.get());
}

TEST_F(ComponentsHierarchyShould, properlyHandleBackwardFocusChangedEvent)
{
    buildComponentsTree();

    for (auto* spy : allSpies_)
    {
        spy->makeSpyFocusable();
    }

    for (size_t index = allSpies_.size(); --index > 0;)
    {
        expectFocusGainedOnlyOn(allSpies_[index]);
        if (index < allSpies_.size()-1)
        {
            expectFocusLostOnlyOn(allSpies_[index+1]);
        }
        sut_->receive(gui::event::FocusChange{.type=gui::event::FocusChange::Type::Previous});
        verifyAndClearAllMocks();
    }
}

/*******************************
      BOUNDS HANDLING TESTS
********************************/
TEST(ComponentShould, setAndGetItsSize)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto required_size = sf::Vector2f{2.f, 9.f};
    sut_->setSize(required_size);

    EXPECT_EQ(sut_->getSize(), required_size);
}

TEST(ComponentShould, KnowOwnPositionWithoutParent)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();

    sut_->setPosition(sf::Vector2f{10.f,20.f});

    auto expected_position = sf::Vector2f{10.f, 20.f};
    EXPECT_EQ(sut_->getPosition(), expected_position);
    EXPECT_EQ(sut_->getGlobalPosition(), expected_position);
}

TEST(ComponentShould, KnowOwnPositionWithParent)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto parent_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();

    parent_->setPosition(sf::Vector2f{10.f,20.f});
    sut_->setPosition(sf::Vector2f{20.f,10.f});
    auto sut_ptr = sut_.get();
    parent_->addChild(std::move(sut_));

    auto expected_global_position = sf::Vector2f{30.f, 30.f};
    auto expected_local_position = sf::Vector2f{20.f, 10.f};
    EXPECT_EQ(sut_ptr->getPosition(), expected_local_position);
    EXPECT_EQ(sut_ptr->getGlobalPosition(), expected_global_position);
}

TEST(ComponentShould, DetectIfPointIsInsideOfComponent)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    sut_->setPosition(sf::Vector2f{20.f,10.f});
    sut_->setSize(sf::Vector2f{10.f,10.f});

    EXPECT_TRUE(sut_->isInside(sf::Vector2f{25.f, 15.f}));
    EXPECT_FALSE(sut_->isInside(sf::Vector2f{0.f, 5.f}));
}

}  // namespace gui
