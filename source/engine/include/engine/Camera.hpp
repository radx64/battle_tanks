#pragma once

#include "engine/Vector2.hpp"

namespace engine 
{

class Camera
{
public:
    Camera(const engine::Vector2f& position, const engine::Vector2f& viewportSize, const engine::Vector2f& worldSize);
    void setPosition(const float x, const float y);
    void resetZoom();
    void moveX(const float xVelocity);
    void moveY(const float yVelocity);
    void move(const float xVelocity, const float yVelocity);
    void zoomIn();
    void zoomIn(const float x, const float y);
    void zoomOut();
    const engine::Vector2f& getPosition();
    const engine::Vector2f& getSize();
    void update(float time_step);
    float getZoom();

protected:
    void alignBoundaries();
    engine::Vector2f currentPosition_;
    engine::Vector2f targetPosition_;
    engine::Vector2f velocity_;
    const engine::Vector2f originalSize_;
    const engine::Vector2f worldSize_;
    engine::Vector2f currentSize_;
    engine::Vector2f targetSize_;
    float zoomLevel_;
};

}  // namespace engine
