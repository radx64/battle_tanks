#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/GuiController.hpp"

#include "tests/mocks/WidgetSpy.hpp"

namespace gui
{

using WidgetSpy = mocks::WidgetSpy;

class GuiControllerFocusHandlingShould : public ::testing::Test
{
public:
    GuiControllerFocusHandlingShould()
    : sut_{{100.f, 100.f}}
    { }

/*
    Widgets hierarchy: (all focusable)
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
    std::unique_ptr<::testing::NiceMock<WidgetSpy>> buildWidgetsTree()
    {
        auto root = std::make_unique<::testing::NiceMock<WidgetSpy>>();
        auto child_1 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
        auto child_1_1 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
        auto child_1_2 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
        auto child_2 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
        auto child_2_1 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
        auto child_2_2 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
        auto child_2_3 = std::make_unique<::testing::NiceMock<WidgetSpy>>();
        auto child_3 = std::make_unique<::testing::NiceMock<WidgetSpy>>();

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

    void expectFocusGainedOnlyOn(WidgetSpy* expected)
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

    void expectFocusLostOnlyOn(WidgetSpy* expected)
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

    WidgetSpy* root_ptr_;
    WidgetSpy* child_1_ptr_;
    WidgetSpy* child_1_1_ptr_;
    WidgetSpy* child_1_2_ptr_;
    WidgetSpy* child_2_ptr_;
    WidgetSpy* child_2_1_ptr_;
    WidgetSpy* child_2_2_ptr_;
    WidgetSpy* child_2_3_ptr_;
    WidgetSpy* child_3_ptr_;

    std::vector<WidgetSpy*> allSpies_;
    GuiController sut_;
};

TEST_F(GuiControllerFocusHandlingShould, properlyFocusElementsGoingForward)
{
    sut_.mainWindow().addChild(buildWidgetsTree());

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
    sut_.mainWindow().addChild(buildWidgetsTree());

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

TEST_F(GuiControllerFocusHandlingShould, skipNonFocusableWidgetGoingForward)
{
    sut_.mainWindow().addChild(buildWidgetsTree());

    root_ptr_->disableFocus();

    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus main window

    EXPECT_CALL(*child_1_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 1st child (root has been skipped)

    EXPECT_CALL(*child_1_ptr_, focusLost());
    EXPECT_CALL(*child_1_1_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Next}); // focus 1.1 child
}

TEST_F(GuiControllerFocusHandlingShould, skipNonFocusableWidgetGoingBackwards)
{
    sut_.mainWindow().addChild(buildWidgetsTree());

    child_3_ptr_->disableFocus();

    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus main window

    EXPECT_CALL(*child_2_3_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 2.3 child (child 3 was skipped)
    
    EXPECT_CALL(*child_2_3_ptr_, focusLost());
    EXPECT_CALL(*child_2_2_ptr_, focusGained());
    sut_.receive(event::FocusChange{.type = event::FocusChange::Type::Previous}); // focus 2.2 child
}

}  // namespace gui
