#ifndef ENGINE_RENDERER_HPP_
#define ENGINE_RENDERER_HPP_

#include <SFML/Graphics.hpp>

namespace engine 
{

class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual void draw(sf::RenderWindow& renderWindow) = 0;
};

}  // namespace engine

#endif  // ENGINE_RENDERER_HPP_