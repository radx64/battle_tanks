#pragma once


#include "gui/Alignment.hpp"
#include "gui/Label.hpp"
#include "gui/Window.hpp"

constexpr std::string_view help_text_string{
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

        auto help_text = std::make_unique<gui::Label>(help_text_string.data());
        help_text->setPosition(sf::Vector2f{0.0f, 0.0f});
        help_text->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        addChild(std::move(help_text));
    }
};

}  // namespace game

