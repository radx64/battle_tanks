#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <utility>

#include <SFML/Graphics.hpp>

namespace engine
{

class ChunkedRenderTextureLayer
{
public:
    using ChunkCoordinates = std::pair<int, int>;

    struct Chunk
    {
        sf::Vector2f position;
        std::unique_ptr<sf::RenderTexture> target;
        sf::Sprite sprite;
        bool needsDisplay{false};
    };

    explicit ChunkedRenderTextureLayer(const int chunkSize);

    void clear();

    Chunk& getOrCreateChunk(const ChunkCoordinates& coordinates);

    template<typename InitializeChunk>
    Chunk& getOrCreateChunk(const ChunkCoordinates& coordinates, InitializeChunk initializeChunk)
    {
        auto [it, inserted] = chunks_.try_emplace(coordinates);
        auto& chunk = it->second;

        if (inserted)
        {
            chunk = createChunk(coordinates);
            initializeChunk(chunk);
            chunk.target->display();
            chunk.sprite.setTexture(chunk.target->getTexture(), true);
            chunk.sprite.setPosition(chunk.position);
        }

        return chunk;
    }

    ChunkCoordinates getChunkCoordinates(float x, float y) const;

    sf::FloatRect getChunkBounds(const ChunkCoordinates& coordinates) const;

    int getChunkSize() const;

    template<typename VisitChunk>
    void forEachExistingChunkIntersecting(const sf::FloatRect& bounds, VisitChunk visitChunk)
    {
        for (auto& [coordinates, chunk] : chunks_)
        {
            if (bounds.intersects(getChunkBounds(coordinates)))
            {
                visitChunk(coordinates, chunk);
            }
        }
    }

    template<typename VisitCoordinates>
    void forEachChunkCoordinateIntersecting(const sf::FloatRect& bounds, VisitCoordinates visitCoordinates) const
    {
        const auto firstChunk = getChunkCoordinates(bounds.left, bounds.top);
        const auto lastChunk = getChunkCoordinates(bounds.left + bounds.width, bounds.top + bounds.height);
        forEachChunkCoordinateIntersecting(bounds, firstChunk, lastChunk, visitCoordinates);
    }

    template<typename VisitCoordinates>
    void forEachChunkCoordinateIntersecting(
        const sf::FloatRect& bounds,
        const ChunkCoordinates& minCoordinates,
        const ChunkCoordinates& maxCoordinates,
        VisitCoordinates visitCoordinates) const
    {
        const auto firstChunk = getChunkCoordinates(bounds.left, bounds.top);
        const auto lastChunk = getChunkCoordinates(bounds.left + bounds.width, bounds.top + bounds.height);

        const auto minChunkX = std::max(minCoordinates.first, firstChunk.first);
        const auto minChunkY = std::max(minCoordinates.second, firstChunk.second);
        const auto maxChunkX = std::min(maxCoordinates.first, lastChunk.first);
        const auto maxChunkY = std::min(maxCoordinates.second, lastChunk.second);

        for (auto chunkY = minChunkY; chunkY <= maxChunkY; ++chunkY)
        {
            for (auto chunkX = minChunkX; chunkX <= maxChunkX; ++chunkX)
            {
                const ChunkCoordinates coordinates{chunkX, chunkY};
                if (bounds.intersects(getChunkBounds(coordinates)))
                {
                    visitCoordinates(coordinates);
                }
            }
        }
    }

private:
    Chunk createChunk(const ChunkCoordinates& coordinates);

    int chunkSize_;
    std::map<ChunkCoordinates, Chunk> chunks_;
};

} // namespace engine
