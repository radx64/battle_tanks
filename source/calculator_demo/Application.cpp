#include "Application.hpp"

//Todo this config need to be dropped
#include "Config.hpp"

#include "gui/Button.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/TextureLibrary.hpp"
#include "gui/layout/Constraint.hpp"

#include "gui/layout/Vertical.hpp"
#include "gui/layout/Horizontal.hpp"
#include "gui/layout/Grid.hpp"
#include "gui/EditBox.hpp"
#include "gui/Label.hpp"
#include "gui/layout/Inset.hpp"

namespace calculator
{
Application::Application()
: gui::Application{"Calculator", "Calculator", sf::Vector2f{600.f, 500.f}}
{}

void Application::onInit()
{
    auto verticalLayout = gui::layout::Vertical::create();
    verticalLayout->setSize(windowManager_.mainWindow().getSize());

    /* EDIT BOX*/
    auto editBox = gui::EditBox::create();
    editBox->setAlignment(gui::Alignment::Right);
    verticalLayout->addChild(std::move(editBox));
    verticalLayout->setRowSize(0, gui::layout::Constraint::Pixels(40));

    auto horizontalLayout = gui::layout::Horizontal::create();
    horizontalLayout->setPadding(10);

    /* TOP BUTTONS */
    auto emptyLabel = gui::Label::create("");
    auto backspaceButton = gui::TextButton::create("Backspace");
    auto CEButton = gui::TextButton::create("CE");
    auto CButton = gui::TextButton::create("C");

    horizontalLayout->addChild(std::move(emptyLabel));
    horizontalLayout->addChild(std::move(backspaceButton));
    horizontalLayout->addChild(std::move(CEButton));
    horizontalLayout->addChild(std::move(CButton));

    auto inset = gui::layout::Inset::create(gui::layout::Constraint::Pixels(20.f));
    inset->addChild(std::move(horizontalLayout));
    verticalLayout->addChild(std::move(inset));
    verticalLayout->setRowSize(1, gui::layout::Constraint::Pixels(60));

    /* 2D KEYBOARD */

    auto gridLayout = gui::layout::Grid::create(5, 4);
    gridLayout->setHorizontalPadding(15);
    gridLayout->setVerticalPadding(15);

    auto button7 = gui::TextButton::create("7");
    auto button8 = gui::TextButton::create("8");
    auto button9 = gui::TextButton::create("9");
    auto buttonDiv = gui::TextButton::create("/");
    auto buttonSqrt = gui::TextButton::create("sqrt");

    gridLayout->addChild(std::move(button7));
    gridLayout->addChild(std::move(button8));
    gridLayout->addChild(std::move(button9));
    gridLayout->addChild(std::move(buttonDiv));
    gridLayout->addChild(std::move(buttonSqrt));

    auto button4 = gui::TextButton::create("4");
    auto button5 = gui::TextButton::create("5");
    auto button6 = gui::TextButton::create("6");
    auto buttonMul = gui::TextButton::create("*");
    auto buttonPercent = gui::TextButton::create("%");

    gridLayout->addChild(std::move(button4));
    gridLayout->addChild(std::move(button5));
    gridLayout->addChild(std::move(button6));
    gridLayout->addChild(std::move(buttonMul));
    gridLayout->addChild(std::move(buttonPercent));

    auto button1 = gui::TextButton::create("1");
    auto button2 = gui::TextButton::create("2");
    auto button3 = gui::TextButton::create("3");
    auto buttonSub = gui::TextButton::create("-");
    auto button1overX = gui::TextButton::create("1/x");

    gridLayout->addChild(std::move(button1));
    gridLayout->addChild(std::move(button2));
    gridLayout->addChild(std::move(button3));
    gridLayout->addChild(std::move(buttonSub));
    gridLayout->addChild(std::move(button1overX));

    auto button0 = gui::TextButton::create("0");
    auto buttonPlusMinus = gui::TextButton::create("+/-");
    auto buttonDot = gui::TextButton::create(".");
    auto buttonAdd = gui::TextButton::create("+");
    auto buttonEqual = gui::TextButton::create("=");
    gridLayout->addChild(std::move(button0));
    gridLayout->addChild(std::move(buttonPlusMinus));
    gridLayout->addChild(std::move(buttonDot));
    gridLayout->addChild(std::move(buttonAdd));
    gridLayout->addChild(std::move(buttonEqual));

    verticalLayout->addChild(std::move(gridLayout));
    windowManager_.mainWindow().addChild(std::move(verticalLayout));
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
    windowManager_.render(window_);
}

void Application::onEvent(const sf::Event& event)
{
    (void) event;
}

}  // namespace calculator
