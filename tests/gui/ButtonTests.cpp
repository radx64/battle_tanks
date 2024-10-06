#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/Button.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{

class ButtonShould : public ::testing::Test
{
protected:
    void SetUp()
    {
        gui::FontLibrary::initialize();
    }

    void TearDown()
    {
        gui::FontLibrary::destroy();
        // FIXME: this code should not have been here
        // Proper resource management should keep everything
        // up to date even if font library is reinitialized
        gui::BasicStyleSheetFactory::destroy();
    }
};

class ActionReceiver
{
public:
    MOCK_METHOD(void, action, ());
};

TEST_F(ButtonShould, DISABLED_callOnClickWhenClickedInside)
{
    ActionReceiver receiver;
    auto sut = std::make_unique<gui::Button>("TEST button");
    sut->setSize(sf::Vector2f{10.f, 10.f});
    sut->setPosition(sf::Vector2f{0.f,0.f});
    sut->onClick([&receiver](){receiver.action();});
    EXPECT_CALL(receiver, action());
    sut->receive(gui::event::MouseButtonPressed{.button = gui::event::MouseButton::Left, .position = {1.f, 1.f}});
}

TEST_F(ButtonShould, DISABLED_dontCallOnClickWhenClickedOutside)
{
    ActionReceiver receiver;
    auto sut = std::make_unique<gui::Button>("TEST button");
    sut->setSize(sf::Vector2f{10.f, 10.f});
    sut->setPosition(sf::Vector2f{0.f,0.f});
    sut->onClick([&receiver](){receiver.action();});
    EXPECT_CALL(receiver, action()).Times(0);
    sut->receive(gui::event::MouseButtonPressed{.button = gui::event::MouseButton::Left, .position = {20.f, 20.f}});
}

}  // namespace gui
