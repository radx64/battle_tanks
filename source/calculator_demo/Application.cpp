#include "Application.hpp"

//Todo this config need to be dropped
#include "Config.hpp"


#include "gui/Button.hpp"
#include "gui/EditBox.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/Label.hpp"
#include "gui/layout/Constraint.hpp"
#include "gui/layout/Grid.hpp"
#include "gui/layout/Horizontal.hpp"
#include "gui/layout/Inset.hpp"
#include "gui/layout/Vertical.hpp"
#include "gui/TextureLibrary.hpp"

#include "calculator_demo/Calculator.hpp"

namespace calculator
{
Application::Application()
: gui::Application{"Calculator", "Calculator", sf::Vector2f{600.f, 500.f}}
{}

void Application::onInit()
{
    windowManager_.mainWindow().addChild(createCalculator(windowManager_.mainWindow().getSize()));
}

void Application::onClose()
{
}

void Application::onUpdate(float timeStep)
{
    (void) timeStep;
}

void Application::onRender()
{
    window_.clear(sf::Color(192, 192, 192));
    gui::Application::onRender();
}

void Application::onEvent(const sf::Event& event)
{
    (void) event;
}

}  // namespace calculator
