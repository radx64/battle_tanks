#ifndef GRAPHICS_PARTICLES_HPP_
#define GRAPHICS_PARTICLES_HPP_

#include <SFML/Graphics.hpp>

namespace graphics
{

class Particles
{
public:
    Particles();
    // Temporary 'particle system' for drawing track imprints
    void add_particle(const int x, const int y, const double angle);
    void draw(sf::RenderWindow& renderWindow);
    void clear();

protected:
sf::RenderTexture target_;
};

}  // namespace graphics

#endif  // GRAPHICS_PARTICLES_HPP_