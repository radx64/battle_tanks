#pragma once

#include <SFML/Graphics.hpp>

namespace engine 
{

class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual void draw(sf::RenderWindow& render_window) = 0;
};

}  // namespace engine
