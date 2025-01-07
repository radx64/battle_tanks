#pragma once

#include "gui/Alignment.hpp"
#include "gui/Label.hpp"
#include "gui/Window.hpp"
#include "gui/Layout.hpp"

constexpr std::string_view helpTextString{
    "WASD - moves view\n"
    "PgUp/PgDn - zoom\n"
    "C - clear all waypoints\n"
    "F8 - slow motion 0.1\n"
    "F9 - slow motion 0.5\n"
    "F10 - normal time step\n"
    "F11 - toggle RIGID BODY debug mode\n"
    "F12 - toggle TANK debug mode\n"
    "F - delete last waypoint\n"
    "T - clear tracks\n"
    "Q - quit\n"};

namespace game
{

class HelpWindow : public gui::Window
{
public:
    HelpWindow(sf::Vector2f position)
    {
        setSize(sf::Vector2f(500.0f, 400.0f));
        setPosition(position);

        auto fillLayout = std::make_unique<gui::FillLayout>();
        auto helpText = std::make_unique<gui::Label>(helpTextString.data());
        helpText->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        fillLayout->addChild(std::move(helpText));

        addChild(std::move(fillLayout));
    }
};

}  // namespace game
