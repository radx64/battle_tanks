#include "math/Math.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace math
{

float distance(float x1, float y1, float x2, float y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float signed_fmod(float a, float n)
{
    return a - floor(a/n) * n;
}

float degree_to_radians(float degree)
{
  return degree * M_PI / 180.0f;
}

sf::Vector2f rotate_point(sf::Vector2f point_to_rotate, float angle, sf::Vector2f pivot)
{
  float radians = degree_to_radians(angle);
  float s = std::sin(radians);
  float c = std::cos(radians);

  // translate point back to point_to_rotate
  point_to_rotate.x -= pivot.x;
  point_to_rotate.y -= pivot.y;

  // rotate point
  float xnew = point_to_rotate.x * c - point_to_rotate.y * s;
  float ynew = point_to_rotate.x * s + point_to_rotate.y * c;

  // translate point back
  point_to_rotate.x = xnew + pivot.x;
  point_to_rotate.y = ynew + pivot.y;
  return point_to_rotate;
}

Average::Average(const size_t window_size)
  : window_size_{window_size}
{
    if(window_size_ < 2) 
    {
      throw std::invalid_argument{"Window size is too small!"};
    }
    measurements_.resize(window_size_);
}

int Average::calculate(int new_value)
{
    measurements_[current_index_] = new_value;
    current_index_++;
    if (current_index_ >= window_size_) current_index_=0;

    return std::accumulate(measurements_.begin(), measurements_.end(), 0) / window_size_;
}

}  // namespace math