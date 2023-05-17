#include "Camera.hpp"

constexpr float MOVE_FACTOR = 0.1;
constexpr float ZOOM_FACTOR = 0.1;

constexpr float MIN_ZOOM_LEVEL = 1;
constexpr float MAX_ZOOM_LEVEL = 3;

Camera::Camera(const sf::Vector2f& position, const sf::Vector2f& size)
: current_position_{position}
, target_position_{position}
, original_size_{size}
, current_size_{size}
, target_size_{size}
, zoom_level_{MIN_ZOOM_LEVEL}
{}

void Camera::set_position(const float x, const float y)
{
    target_position_.x = x;
    target_position_.y = y;
    align_boundaries();
}

void Camera::align_boundaries()
{
    auto half_of_target_size = target_size_/2.0f;
    if (target_position_.x - half_of_target_size.x < 0) target_position_.x = half_of_target_size.x;
    if (target_position_.y - half_of_target_size.y < 0) target_position_.y = half_of_target_size.y;

    if (target_position_.x + half_of_target_size.x > original_size_.x) target_position_.x = original_size_.x - half_of_target_size.x;
    if (target_position_.y + half_of_target_size.y > original_size_.y) target_position_.y = original_size_.y - half_of_target_size.y;
}

void Camera::move(const float x_offset, const float y_offset)
{
    target_position_.x += x_offset;
    target_position_.y += y_offset;
    align_boundaries();
}

void Camera::zoom_in()
{
    if (zoom_level_ < MAX_ZOOM_LEVEL) zoom_level_+=0.5f;
    else {return;}

    target_size_.x = original_size_.x / zoom_level_;
    target_size_.y = original_size_.y / zoom_level_;
    align_boundaries();
}

void Camera::zoom_out()
{
    if (zoom_level_ > MIN_ZOOM_LEVEL) zoom_level_-=0.5f;
    else {return;}  

    target_size_.x = original_size_.x / zoom_level_;
    target_size_.y = original_size_.y / zoom_level_;
    align_boundaries();
}

const sf::Vector2f& Camera::get_position()
{
    return current_position_;
}

const sf::Vector2f& Camera::get_size()
{
    return current_size_;
}

void Camera::physics()
{
    const auto position_diff = target_position_ - current_position_;
    current_position_ += position_diff  * MOVE_FACTOR;

    const auto zoom_diff = target_size_ - current_size_;
    current_size_ += zoom_diff  * ZOOM_FACTOR;
}