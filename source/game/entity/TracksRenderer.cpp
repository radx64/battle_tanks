#include "game/entity/TracksRenderer.hpp"

#include "engine/Camera.hpp"
#include "engine/Context.hpp"

namespace game::entity
{

constexpr auto TRACK_WIDTH = 2;
constexpr auto TRACK_HEIGHT = 6;
constexpr auto TRACK_BOUNDING_RADIUS = TRACK_HEIGHT + TRACK_WIDTH;
constexpr auto TRACK_IMPRINT_OPACITY = 50;
constexpr auto PARTICLE_LAYER_OPACITY = 90;
constexpr auto CHUNK_SIZE = 1024;

TracksRenderer::TracksRenderer()
: chunks_{CHUNK_SIZE}
{}

void TracksRenderer::addTrackImprint(const int x, const int y, const float angle)
{
    const auto imprintBounds = sf::FloatRect(
        static_cast<float>(x - TRACK_BOUNDING_RADIUS),
        static_cast<float>(y - TRACK_BOUNDING_RADIUS),
        static_cast<float>(TRACK_BOUNDING_RADIUS * 2),
        static_cast<float>(TRACK_BOUNDING_RADIUS * 2));

    chunks_.forEachChunkCoordinateIntersecting(
        imprintBounds,
        [this, x, y, angle](const auto& coordinates)
        {
            auto& chunk = chunks_.getOrCreateChunk(coordinates);

            sf::RectangleShape rectangle(sf::Vector2f(TRACK_HEIGHT, TRACK_WIDTH));
            rectangle.setPosition(sf::Vector2f(x, y) - chunk.position);
            rectangle.setFillColor(sf::Color(0, 0, 0, TRACK_IMPRINT_OPACITY));
            rectangle.setRotation(angle);

            chunk.target->draw(rectangle);
            chunk.needsDisplay = true;
        });
}

void TracksRenderer::draw(sf::RenderWindow& renderWindow)
{
    auto& camera = engine::Context::getCamera();
    const auto& cameraPosition = camera.getPosition();
    const sf::FloatRect cameraFrustum{cameraPosition - camera.getSize() / 2.f, camera.getSize()};

    chunks_.forEachExistingChunkIntersecting(
        cameraFrustum,
        [&renderWindow](const auto&, auto& chunk)
        {
            if (chunk.needsDisplay)
            {
                chunk.target->display();
                chunk.needsDisplay = false;
                chunk.sprite.setTexture(chunk.target->getTexture(), true);
            }

            chunk.sprite.setColor(sf::Color(0, 0, 0, PARTICLE_LAYER_OPACITY));
            renderWindow.draw(chunk.sprite);
        });
}

void TracksRenderer::clear()
{
    chunks_.clear();
}

} // namespace game::entity
