#include "Application.hpp"

//Todo this config need to be dropped
#include "Config.hpp"


#include "engine/gui/Button.hpp"
#include "engine/gui/EditBox.hpp"
#include "engine/gui/FontLibrary.hpp"
#include "engine/gui/Label.hpp"
#include "engine/gui/layout/Constraint.hpp"
#include "engine/gui/layout/Grid.hpp"
#include "engine/gui/layout/Horizontal.hpp"
#include "engine/gui/layout/Inset.hpp"
#include "engine/gui/layout/Vertical.hpp"
#include "engine/gui/TextureLibrary.hpp"

#include "calculator_demo/Calculator.hpp"

namespace calculator
{
Application::Application()
: engine::gui::Application{"Calculator", "Calculator", sf::Vector2f{600.f, 500.f}}
{}

void Application::onInit()
{
    gui().mainWindow().addChild(createCalculator(gui().mainWindow().getSize()));
}

void Application::onClose()
{
}

void Application::onUpdate(float time_step)
{
    (void) time_step;
}

void Application::onRender()
{
    window_.clear(sf::Color(192, 192, 192));
    engine::gui::Application::onRender();
}

void Application::onEvent(const sf::Event& event)
{
    (void) event;
}

}  // namespace calculator
