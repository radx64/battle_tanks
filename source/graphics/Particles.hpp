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
    void addParticle(const int x, const int y, const float angle);
    void draw(sf::RenderWindow& renderWindow);
    void clear();

protected:
sf::RenderTexture target_;
};

}  // namespace graphics

#endif  // GRAPHICS_PARTICLES_HPP_