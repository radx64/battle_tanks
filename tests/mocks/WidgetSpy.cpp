#include "WidgetSpy.hpp"

namespace mocks
{

WidgetSpy::~WidgetSpy()
{
    logger_.debug("Dying");
    die();
}   

void WidgetSpy::onRender(engine::gfx::RenderTarget&)
{
    logger_.debug("onRender called");
    render_mock();
}

engine::gui::EventStatus WidgetSpy::on(const engine::gui::events::FocusGained&)
{
    logger_.debug("Focus gained");
    focusGained();
    return engine::gui::EventStatus::Consumed;
}

engine::gui::EventStatus WidgetSpy::on(const engine::gui::events::FocusLost&)
{
    logger_.debug("Focus lost");
    focusLost();
    return engine::gui::EventStatus::Consumed;
}  
 
void WidgetSpy::makeSpyFocusable()
{
    logger_.debug("Spy made focusable");
    enableFocus();
}

}  // namespace mocks
