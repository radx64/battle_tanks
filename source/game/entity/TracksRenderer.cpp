#include "game/entity/TracksRenderer.hpp"

namespace game::entity
{

constexpr auto TRACK_WIDTH = 2;
constexpr auto TRACK_HEIGHT = 6;
constexpr auto TRACK_IMPRINT_OPACITY = 50;
constexpr auto PARTICLE_LAYER_OPACITY = 90;

TracksRenderer::TracksRenderer()
{
    // FIXME: This layer should be resized alongside window resolution
    target_.create(1920, 1080);
    sprite_.setTexture(target_.getTexture());
    needsDisplay_ = true;
}

void TracksRenderer::addTrackImprint(const int x, const int y, const float angle)
{
    sf::RectangleShape rectangle(sf::Vector2f(TRACK_HEIGHT, TRACK_WIDTH));
    rectangle.setPosition(sf::Vector2f(x, y));
    rectangle.setFillColor(sf::Color(0, 0, 0, TRACK_IMPRINT_OPACITY));
    rectangle.setRotation(angle);
    target_.draw(rectangle);
    needsDisplay_ = true;
}

void TracksRenderer::draw(sf::RenderWindow& renderWindow)
{
    if (needsDisplay_)
    {
        target_.display();
        needsDisplay_ = false;
        sprite_.setTexture(target_.getTexture());
    }

    sprite_.setColor(sf::Color(0, 0, 0, PARTICLE_LAYER_OPACITY));
    renderWindow.draw(sprite_);
}

void TracksRenderer::clear()
{
    target_.clear(sf::Color(255,255,255,0));
    needsDisplay_ = true;
}

} // namespace game::entity
