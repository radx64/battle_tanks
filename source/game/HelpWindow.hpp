#pragma once

#include "engine/gui/Alignment.hpp"
#include "engine/gui/Label.hpp"
#include "engine/gui/Window.hpp"

constexpr std::string_view helpTextString{
    "WASD - moves view\n"
    "PgUp/PgDn - zoom\n"
    "C - clear all waypoints\n"
    "F8 - slow motion 10x slower\n"
    "F9 - slow motion 5x slower\n"
    "F10 - normal time step\n"
    "F11 - toggle RIGID BODY debug mode\n"
    "F12 - toggle TANK debug mode\n"
    "F - delete last waypoint\n"
    "T - clear tracks\n"
    "Q - quit\n"};

namespace game
{

class HelpWindow : public engine::gui::Window
{
public:
    HelpWindow(sf::Vector2f position)
    {
        setSize(sf::Vector2f(300.0f, 300.0f));
        setPosition(position);
        auto help_text = engine::gui::Label::create(helpTextString.data());
        help_text->setAlignment(engine::gui::Alignment::HorizontallyCentered | engine::gui::Alignment::VerticallyCentered);
        addChild(std::move(help_text));
    }
};

}  // namespace game
