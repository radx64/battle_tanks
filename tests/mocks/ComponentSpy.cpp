#include "ComponentSpy.hpp"

namespace mocks
{

ComponentSpy::~ComponentSpy()
{
    logger_.debug("Dying");
    die();
}   

void ComponentSpy::onRender(sf::RenderTexture&)
{
    logger_.debug("onRender called");
    render_mock();
}

gui::EventStatus ComponentSpy::on(const gui::event::FocusGained&)
{
    logger_.debug("Focus gained");
    focusGained();
    return gui::EventStatus::Consumed;
}

gui::EventStatus ComponentSpy::on(const gui::event::FocusLost&)
{
    logger_.debug("Focus lost");
    focusLost();
    return gui::EventStatus::Consumed;
}  
 
void ComponentSpy::makeSpyFocusable()
{
    logger_.debug("Spy made focusable");
    enableFocus();
}

}  // namespace mocks
