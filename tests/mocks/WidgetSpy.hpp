
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "engine/gui/Widget.hpp"

namespace mocks
{

class WidgetSpy : public engine::gui::Widget
{
public:
    MOCK_METHOD(void, die, ());
    MOCK_METHOD(void, render_mock, ());
    MOCK_METHOD(void, focusGained, ());
    MOCK_METHOD(void, focusLost, ());

    ~WidgetSpy() override;

    void onRender(sf::RenderTexture&) override;
    engine::gui::EventStatus on(const engine::gui::event::FocusGained&) override;
    engine::gui::EventStatus on(const engine::gui::event::FocusLost&) override;

    void makeSpyFocusable();
};

}  // namespace mocks
