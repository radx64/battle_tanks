#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

namespace graphics
{

class Tilemap
{
public:
    Tilemap();
    ~Tilemap();

    void draw(sf::RenderWindow& render_window);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace graphics
