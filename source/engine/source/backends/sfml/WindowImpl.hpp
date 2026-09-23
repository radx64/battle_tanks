#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "engine/Window.hpp"

namespace engine
{

class Window::Impl
{
public:
    Impl(const WindowProperties& properties);
    sf::RenderWindow renderWindow_;
};

}  // namespace engine
