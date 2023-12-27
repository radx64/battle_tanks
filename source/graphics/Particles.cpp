#include "graphics/Particles.hpp"

namespace graphics
{

constexpr auto TRACK_WIDTH = 6;
constexpr auto TRACK_HEIGHT = 2;
constexpr auto TRACK_IMPRINT_OPACITY = 50;
constexpr auto PARTICLE_LAYER_OPACITY = 90;

Particles::Particles()
{
    // FIXME: This layer should be resized alongside window resolution
    target_.create(1920, 1080);
}

// Temporary 'particle system' for drawing track imprints
void Particles::addParticle(const int x, const int y, const float angle)
{
    sf::RectangleShape rectangle(sf::Vector2f(TRACK_HEIGHT, TRACK_WIDTH));
    rectangle.setPosition(sf::Vector2f(x, y));
    rectangle.setFillColor(sf::Color(0, 0, 0, TRACK_IMPRINT_OPACITY));
    rectangle.setRotation(angle);  
    target_.draw(rectangle);
    target_.display();
}

void Particles::draw(sf::RenderWindow& renderWindow)
{
    sf::Sprite texture(target_.getTexture());
    texture.setColor(sf::Color(0, 0, 0, PARTICLE_LAYER_OPACITY));
    renderWindow.draw(texture);
}

void Particles::clear()
{
    target_.clear(sf::Color(255,255,255,0));
}

}  // namespace graphics
