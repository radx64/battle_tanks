#include "WidgetSpy.hpp"

namespace mocks
{

WidgetSpy::~WidgetSpy()
{
    logger_.debug("Dying");
    die();
}   

void WidgetSpy::onRender(sf::RenderTexture&)
{
    logger_.debug("onRender called");
    render_mock();
}

engine::gui::EventStatus WidgetSpy::on(const engine::gui::event::FocusGained&)
{
    logger_.debug("Focus gained");
    focusGained();
    return engine::gui::EventStatus::Consumed;
}

engine::gui::EventStatus WidgetSpy::on(const engine::gui::event::FocusLost&)
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
