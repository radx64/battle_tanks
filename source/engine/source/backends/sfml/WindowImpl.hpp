#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include "SfmlRenderTarget.hpp"
#include "engine/Window.hpp"
#include "engine/gfx/View.hpp"

namespace engine
{

class Window::Impl
{
public:
    Impl(const WindowProperties& properties);

    void syncDefaultView();

    sf::RenderWindow renderWindow_;
    std::unique_ptr<engine::backends::sfml::SfmlRenderTarget> renderTarget_;
    engine::gfx::View defaultView_{};
};

}  // namespace engine
