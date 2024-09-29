#include "engine/Camera.hpp"

namespace engine 
{

constexpr float MOVE_FACTOR = 1.1;
constexpr float ZOOM_FACTOR = 1.1;

constexpr float MIN_ZOOM_LEVEL = 1;
constexpr float MAX_ZOOM_LEVEL = 4;

Camera::Camera(const sf::Vector2f& position, const sf::Vector2f& size)
: currentPosition_{position}
, targetPosition_{position}
, velocity_{0.f, 0.f}
, originalSize_{size}
, currentSize_{size}
, targetSize_{size}
, zoomLevel_{MIN_ZOOM_LEVEL}
{}

void Camera::setPosition(const float x, const float y)
{
    targetPosition_.x = x;
    targetPosition_.y = y;
    alignBoundaries();
}

void Camera::alignBoundaries()
{
    auto halfOfTargetSize = targetSize_/2.0f;
    if (targetPosition_.x - halfOfTargetSize.x < 0) targetPosition_.x = halfOfTargetSize.x;
    if (targetPosition_.y - halfOfTargetSize.y < 0) targetPosition_.y = halfOfTargetSize.y;

    if (targetPosition_.x + halfOfTargetSize.x > originalSize_.x) targetPosition_.x = originalSize_.x - halfOfTargetSize.x;
    if (targetPosition_.y + halfOfTargetSize.y > originalSize_.y) targetPosition_.y = originalSize_.y - halfOfTargetSize.y;
}

void Camera::moveX(const float xVelocity)
{
    velocity_.x = xVelocity;
}

void Camera::moveY(const float yVelocity)
{
    velocity_.y = yVelocity;
}

void Camera::move(const float xVelocity, const float yVelocity)
{
    moveX(xVelocity);
    moveY(yVelocity);
}

// TODO: combine zoomIn and zoomIn (x,y) methods later
void Camera::zoomIn()
{
    if (zoomLevel_ >= MAX_ZOOM_LEVEL) return;
    zoomLevel_+=0.5f;

    targetSize_.x = originalSize_.x / zoomLevel_;
    targetSize_.y = originalSize_.y / zoomLevel_;
    alignBoundaries();
}

void Camera::zoomIn(const float x, const float y)
{
    if (zoomLevel_ >= MAX_ZOOM_LEVEL) return;
    zoomIn();

    auto halfOfTargetSize = targetSize_/2.0f;

    const float localX = x / zoomLevel_;
    const float localY = y / zoomLevel_;

    const float newPositionX  = targetPosition_.x + (localX - halfOfTargetSize.x);
    const float newPositionY  = targetPosition_.y + (localY - halfOfTargetSize.y);
    setPosition(newPositionX, newPositionY);
}

void Camera::zoomOut()
{
    if (zoomLevel_ > MIN_ZOOM_LEVEL) zoomLevel_-=0.5f;
    else {return;}  

    targetSize_.x = originalSize_.x / zoomLevel_;
    targetSize_.y = originalSize_.y / zoomLevel_;
    alignBoundaries();
}

const sf::Vector2f& Camera::getPosition()
{
    return currentPosition_;
}

const sf::Vector2f& Camera::getSize()
{
    return currentSize_;
}

void Camera::update(float timeStep)
{
    targetPosition_ += velocity_ * timeStep;
    alignBoundaries();

    const auto positionDiff = targetPosition_ - currentPosition_;
    currentPosition_ += positionDiff  * MOVE_FACTOR  * timeStep;

    const auto zoomDiff = targetSize_ - currentSize_;
    currentSize_ += zoomDiff  * ZOOM_FACTOR * timeStep;
}

float Camera::getZoom()
{
    return  originalSize_.x / currentSize_.x;
}

}  // namespace engine
