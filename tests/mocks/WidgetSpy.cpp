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

gui::EventStatus WidgetSpy::on(const gui::event::FocusGained&)
{
    logger_.debug("Focus gained");
    focusGained();
    return gui::EventStatus::Consumed;
}

gui::EventStatus WidgetSpy::on(const gui::event::FocusLost&)
{
    logger_.debug("Focus lost");
    focusLost();
    return gui::EventStatus::Consumed;
}  
 
void WidgetSpy::makeSpyFocusable()
{
    logger_.debug("Spy made focusable");
    enableFocus();
}

}  // namespace mocks
