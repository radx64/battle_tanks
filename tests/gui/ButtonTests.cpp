#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/Button.hpp"
#include "gui/FontLibrary.hpp"

namespace gui
{

class ButtonShould : public ::testing::Test
{
protected:
    void SetUp()
    {
        gui::FontLibrary::init();
    }

    void TearDown()
    {
        gui::FontLibrary::destroy();
    }
};

class ActionReceiver
{
public:
    MOCK_METHOD(void, action, ());
};

TEST_F(ButtonShould, callOnClickWhenClickedInside)
{
    ActionReceiver receiver;
    auto sut = gui::TextButton::create("TEST button");
    sut->setSize(sf::Vector2f{10.f, 10.f});
    sut->setPosition(sf::Vector2f{0.f,0.f});
    sut->onClick([&receiver](){receiver.action();});
    EXPECT_CALL(receiver, action());
    sut->receive(gui::event::MouseButtonPressed{.button = gui::event::MouseButton::Left, .position = {1.f, 1.f}});
    sut->receive(gui::event::MouseButtonReleased{.button = gui::event::MouseButton::Left, .position = {1.f, 1.f}});
}

}  // namespace gui
