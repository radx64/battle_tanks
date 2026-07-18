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
        const auto first_chunk = getChunkCoordinates(bounds.left, bounds.top);
        const auto last_chunk = getChunkCoordinates(bounds.left + bounds.width, bounds.top + bounds.height);
        forEachChunkCoordinateIntersecting(bounds, first_chunk, last_chunk, visitCoordinates);
    }

    template<typename VisitCoordinates>
    void forEachChunkCoordinateIntersecting(
        const sf::FloatRect& bounds,
        const ChunkCoordinates& minCoordinates,
        const ChunkCoordinates& maxCoordinates,
        VisitCoordinates visitCoordinates) const
    {
        const auto first_chunk = getChunkCoordinates(bounds.left, bounds.top);
        const auto last_chunk = getChunkCoordinates(bounds.left + bounds.width, bounds.top + bounds.height);

        const auto min_chunk_x = std::max(minCoordinates.first, first_chunk.first);
        const auto min_chunk_y = std::max(minCoordinates.second, first_chunk.second);
        const auto max_chunk_x = std::min(maxCoordinates.first, last_chunk.first);
        const auto max_chunk_y = std::min(maxCoordinates.second, last_chunk.second);

        for (auto chunk_y = min_chunk_y; chunk_y <= max_chunk_y; ++chunk_y)
        {
            for (auto chunk_x = min_chunk_x; chunk_x <= max_chunk_x; ++chunk_x)
            {
                const ChunkCoordinates coordinates{chunk_x, chunk_y};
                if (bounds.intersects(getChunkBounds(coordinates)))
                {
                    visitCoordinates(coordinates);
                }
            }
        }
    }

private:
    Chunk createChunk(const ChunkCoordinates& coordinates);

    int chunk_size_;
    std::map<ChunkCoordinates, Chunk> chunks_;
};

} // namespace engine
