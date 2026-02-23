#include "engine/math/Math.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace engine::math
{

float distance(float x1, float y1, float x2, float y2)
{
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float dotProduct(float x1, float y1, float x2, float y2)
{
  return x1 * x2 + y1 * y2;
}

float crossProduct(float x1, float y1, float x2, float y2)
{
  return x1 * y2 - y1 * x2;
}

float normalizeVector(sf::Vector2f& vector)
{
  float norm = std::sqrt(vector.x * vector.x + vector.y * vector.y); 
  if (norm != 0.f) vector /= norm;
  return norm;
}

float signedFmod(float a, float n)
{
  return a - floor(a/n) * n;
}

float degreeToRadians(float degree)
{
  return degree * M_PI / 180.0f;
}

sf::Vector2f rotatePoint(sf::Vector2f point_to_rotate, float angle, sf::Vector2f pivot)
{
  float radians = degreeToRadians(angle);
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

sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t)
{
  return a + (b - a) * t; 
}

float lerp(const float a, const float b, float t)
{
  return a + (b - a) * t; 
}

Average::Average(const size_t window_size)
  : windowSize_{window_size}
{
    if(windowSize_ < 2) 
    {
      throw std::invalid_argument{"Window size is too small!"};
    }
    measurements_.resize(windowSize_);
}

const std::vector<int32_t>& Average::measurements() const
{
  return measurements_;
}

int Average::calculate(int32_t new_value)
{
    measurements_[currentIndex_] = new_value;
    currentIndex_++;
    if (currentIndex_ >= windowSize_) currentIndex_ = 0;
    return std::accumulate(measurements_.begin(), measurements_.end(), 0) / windowSize_;
}

}  // namespace engine::math
