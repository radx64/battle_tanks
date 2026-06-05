#include "engine/ChunkedRenderTextureLayer.hpp"

#include <cmath>

namespace engine
{

ChunkedRenderTextureLayer::ChunkedRenderTextureLayer(const int chunkSize)
: chunkSize_{chunkSize}
{}

void ChunkedRenderTextureLayer::clear()
{
    chunks_.clear();
}

ChunkedRenderTextureLayer::Chunk& ChunkedRenderTextureLayer::getOrCreateChunk(const ChunkCoordinates& coordinates)
{
    auto [it, inserted] = chunks_.try_emplace(coordinates);
    auto& chunk = it->second;

    if (inserted)
    {
        chunk = createChunk(coordinates);
        chunk.target->display();
        chunk.sprite.setTexture(chunk.target->getTexture(), true);
        chunk.sprite.setPosition(chunk.position);
    }

    return chunk;
}

ChunkedRenderTextureLayer::ChunkCoordinates ChunkedRenderTextureLayer::getChunkCoordinates(float x, float y) const
{
    return {
        static_cast<int>(std::floor(x / chunkSize_)),
        static_cast<int>(std::floor(y / chunkSize_))};
}

sf::FloatRect ChunkedRenderTextureLayer::getChunkBounds(const ChunkCoordinates& coordinates) const
{
    return sf::FloatRect(
        static_cast<float>(coordinates.first * chunkSize_),
        static_cast<float>(coordinates.second * chunkSize_),
        static_cast<float>(chunkSize_),
        static_cast<float>(chunkSize_));
}

int ChunkedRenderTextureLayer::getChunkSize() const
{
    return chunkSize_;
}

ChunkedRenderTextureLayer::Chunk ChunkedRenderTextureLayer::createChunk(const ChunkCoordinates& coordinates)
{
    Chunk chunk;
    chunk.position = sf::Vector2f(
        static_cast<float>(coordinates.first * chunkSize_),
        static_cast<float>(coordinates.second * chunkSize_));
    chunk.target = std::make_unique<sf::RenderTexture>();
    chunk.target->create(chunkSize_, chunkSize_);
    chunk.target->clear(sf::Color(255, 255, 255, 0));
    return chunk;
}

} // namespace engine
