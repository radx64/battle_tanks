#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui
{

class ComponentSpy : public Component
{
public:
    MOCK_METHOD(void, die, ());
    ~ComponentSpy() override
    {
        die();
    }

    MOCK_METHOD(void, render_mock, ());
    void onRender(sf::RenderTexture& renderWindow) override
    {
        UNUSED(renderWindow);
        render_mock();
    }

    MOCK_METHOD(void, onParentSizeChange, (const sf::Vector2f& parent_size), (override));
};

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

TEST(ComponentShould, callOnParentSizeChangedForChildrenWhenParentSizeIsChanged)
{
    auto sut_ = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
    auto child_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();

    auto child_1_ptr = child_1.get();
    auto child_2_ptr = child_2.get();

    sut_->addChild(std::move(child_1));
    sut_->addChild(std::move(child_2));

    auto expected_parent_size = sf::Vector2f{0.f, 0.f};

    EXPECT_CALL(*child_1_ptr, onParentSizeChange(expected_parent_size)).Times(1);
    EXPECT_CALL(*child_2_ptr, onParentSizeChange(expected_parent_size)).Times(1);

    sut_->setSize(expected_parent_size);
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
