
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "gui/Component.hpp"

namespace mocks
{

class ComponentSpy : public gui::Component
{
public:
    MOCK_METHOD(void, die, ());
    MOCK_METHOD(void, render_mock, ());
    MOCK_METHOD(void, focusGained, ());
    MOCK_METHOD(void, focusLost, ());

    ~ComponentSpy() override;

    void onRender(sf::RenderTexture&) override;
    gui::EventStatus on(const gui::event::FocusGained&) override;
    gui::EventStatus on(const gui::event::FocusLost&) override;

    void makeSpyFocusable();
};

}  // namespace mocks
