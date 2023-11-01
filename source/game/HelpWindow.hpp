#ifndef GAME_HELPWINDOW_HPP_
#define GAME_HELPWINDOW_HPP_

#include <gui/Alignment.hpp>
#include "gui/Window.hpp"

constexpr std::string_view help_text_string{
    "=== HELP ===\n"
    "WASD - moves view\n" 
    "PgUp/PgDn - zoom\n"  
    "C - clear all waypoints\n"
    "F12 - toggle debug draw mode\n"
    "F - delete last waypoint\n"
    "T - clear tracks\n"
    "Q - quit\n"};

namespace game
{

class HelpWindow : public gui::Window
{
public:
    HelpWindow(sf::Vector2f position) 
    : gui::Window(nullptr) 
    {
        setSize(sf::Vector2f(500.0f, 400.0f));
        setPosition(position, gui::Alignment::CENTERED);

        auto help_text = new gui::Label(help_text_string.data(), this);
        help_text->setPosition(sf::Vector2f(20.0f, 20.0f), gui::Alignment::LEFT);
    }
};

}  // namespace game

#endif  // GAME_HELPWINDOW_HPP_