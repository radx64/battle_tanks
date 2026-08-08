#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "engine/gui/Button.hpp"
#include "engine/gui/FontLibrary.hpp"

namespace engine::gui
{

class ButtonShould : public ::testing::Test
{
protected:
    void SetUp()
    {
        engine::gui::FontLibrary::init();
    }

    void TearDown()
    {
        engine::gui::FontLibrary::destroy();
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
    auto sut = engine::gui::TextButton::create("TEST button");
    sut->setSize(sf::Vector2f{10.f, 10.f});
    sut->setPosition(sf::Vector2f{0.f,0.f});
    sut->onClick([&receiver](){receiver.action();});
    EXPECT_CALL(receiver, action());
    sut->receive(engine::gui::event::MouseButtonPressed{.button = engine::gui::event::MouseButton::Left, .position = {1.f, 1.f}});
    sut->receive(engine::gui::event::MouseButtonReleased{.button = engine::gui::event::MouseButton::Left, .position = {1.f, 1.f}});
}

}  // namespace engine::gui
