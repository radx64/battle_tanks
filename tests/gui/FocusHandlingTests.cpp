#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/GuiController.hpp"

#include "tests/mocks/ComponentSpy.hpp"

namespace gui
{

using ComponentSpy = mocks::ComponentSpy;

class GuiControllerFocusHandlingShould : public ::testing::Test
{
public:
    GuiControllerFocusHandlingShould()
    : sut_{{100.f, 100.f}}
    { }

/*
    Components hierarchy: (all focusable)
    root
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
    std::unique_ptr<::testing::NiceMock<ComponentSpy>> buildComponentsTree()
    {
        auto root = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_1_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_1_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_2_1 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_2_2 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_2_3 = std::make_unique<::testing::NiceMock<ComponentSpy>>();
        auto child_3 = std::make_unique<::testing::NiceMock<ComponentSpy>>();

        root->enableFocus();
        child_1->enableFocus();
        child_1_1->enableFocus();
        child_1_2->enableFocus();
        child_2->enableFocus();
        child_2_1->enableFocus();
        child_2_2->enableFocus();
        child_2_3->enableFocus();
        child_3->enableFocus();

        root_ptr_ = root.get();
        child_1_ptr_ = child_1.get();
        child_1_2_ptr_ = child_1_2.get();
        child_1_1_ptr_ = child_1_1.get();
        child_2_ptr_ = child_2.get();
        child_2_1_ptr_ = child_2_1.get();
        child_2_2_ptr_ = child_2_2.get();
        child_2_3_ptr_ = child_2_3.get();
        child_3_ptr_ = child_3.get();

        allSpies_ = {
            root_ptr_,
            child_1_ptr_,
            child_1_1_ptr_,
            child_1_2_ptr_,
            child_2_ptr_,
            child_2_1_ptr_,
            child_2_2_ptr_,
            child_2_3_ptr_,
            child_3_ptr_,
        };

        root_ptr_->addChild(std::move(child_1));
        root_ptr_->addChild(std::move(child_2));
        root_ptr_->addChild(std::move(child_3));

        child_1_ptr_->addChild(std::move(child_1_1));
        child_1_ptr_->addChild(std::move(child_1_2));

        child_2_ptr_->addChild(std::move(child_2_1));
        child_2_ptr_->addChild(std::move(child_2_2));
        child_2_ptr_->addChild(std::move(child_2_3));

        return root; 
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

    ComponentSpy* root_ptr_;
    ComponentSpy* child_1_ptr_;
    ComponentSpy* child_1_1_ptr_;
    ComponentSpy* child_1_2_ptr_;
    ComponentSpy* child_2_ptr_;
    ComponentSpy* child_2_1_ptr_;
    ComponentSpy* child_2_2_ptr_;
    ComponentSpy* child_2_3_ptr_;
    ComponentSpy* child_3_ptr_;

    std::vector<ComponentSpy*> allSpies_;
    GuiController sut_;
};

TEST_F(GuiControllerFocusHandlingShould, properlyFocusElementsGoingForward)
{
    sut_.mainWindow().addChild(buildComponentsTree());

    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus main window
    EXPECT_CALL(*root_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus root inside of a main window

    EXPECT_CALL(*root_ptr_, focusLost());
    EXPECT_CALL(*child_1_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 1st child

    EXPECT_CALL(*child_1_ptr_, focusLost());
    EXPECT_CALL(*child_1_1_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 1.1 child

    EXPECT_CALL(*child_1_1_ptr_, focusLost());
    EXPECT_CALL(*child_1_2_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 1.2 child

    EXPECT_CALL(*child_1_2_ptr_, focusLost());
    EXPECT_CALL(*child_2_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 2nd child

    EXPECT_CALL(*child_2_ptr_, focusLost());
    EXPECT_CALL(*child_2_1_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 2.1 child

    EXPECT_CALL(*child_2_1_ptr_, focusLost());
    EXPECT_CALL(*child_2_2_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 2.2 child

    EXPECT_CALL(*child_2_2_ptr_, focusLost());
    EXPECT_CALL(*child_2_3_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 2.3 child

    EXPECT_CALL(*child_2_3_ptr_, focusLost());
    EXPECT_CALL(*child_3_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 3 child

    EXPECT_CALL(*child_3_ptr_, focusLost());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // wrap around and focus a main window

    EXPECT_CALL(*root_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus root inside of a window
}

TEST_F(GuiControllerFocusHandlingShould, properlyFocusElementsGoingBackwards)
{
    sut_.mainWindow().addChild(buildComponentsTree());

    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus main window

    EXPECT_CALL(*child_3_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // wrap around and focus last child (3) of main window
    
    EXPECT_CALL(*child_3_ptr_, focusLost());
    EXPECT_CALL(*child_2_3_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 2.3 child
    
    EXPECT_CALL(*child_2_3_ptr_, focusLost());
    EXPECT_CALL(*child_2_2_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 2.2 child

    EXPECT_CALL(*child_2_2_ptr_, focusLost());
    EXPECT_CALL(*child_2_1_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 2.1 child

    EXPECT_CALL(*child_2_1_ptr_, focusLost());
    EXPECT_CALL(*child_2_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 2 child

    EXPECT_CALL(*child_2_ptr_, focusLost());
    EXPECT_CALL(*child_1_2_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 1.2 child

    EXPECT_CALL(*child_1_2_ptr_, focusLost());
    EXPECT_CALL(*child_1_1_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 1.1 child

    EXPECT_CALL(*child_1_1_ptr_, focusLost());
    EXPECT_CALL(*child_1_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 1st child

    EXPECT_CALL(*child_1_ptr_, focusLost());
    EXPECT_CALL(*root_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus root inside of a main window
}

TEST_F(GuiControllerFocusHandlingShould, skipNonFocusableCompoenentGoingForward)
{
    sut_.mainWindow().addChild(buildComponentsTree());

    root_ptr_->disableFocus();

    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus main window

    EXPECT_CALL(*child_1_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 1st child (root has been skipped)

    EXPECT_CALL(*child_1_ptr_, focusLost());
    EXPECT_CALL(*child_1_1_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 1.1 child
}

TEST_F(GuiControllerFocusHandlingShould, skipNonFocusableCompoenentGoingBackwards)
{
    sut_.mainWindow().addChild(buildComponentsTree());

    child_3_ptr_->disableFocus();

    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus main window

    EXPECT_CALL(*child_2_3_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 2.3 child (child 3 was skipped)
    
    EXPECT_CALL(*child_2_3_ptr_, focusLost());
    EXPECT_CALL(*child_2_2_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 2.2 child
}

}  // namespace gui
