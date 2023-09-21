#include "math/Math.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace math
{

double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double signed_fmod(double a, double n)
{
    return a - floor(a/n) * n;
}

double degree_to_radians(double degree)
{
  return degree * M_PI / 180.0;
}

sf::Vector2f rotate_point(sf::Vector2f point_to_rotate, double angle, sf::Vector2f pivot)
{
  double radians = degree_to_radians(angle);
  double s = sin(radians);
  double c = cos(radians);

  // translate point back to point_to_rotate
  point_to_rotate.x -= pivot.x;
  point_to_rotate.y -= pivot.y;

  // rotate point
  double xnew = point_to_rotate.x * c - point_to_rotate.y * s;
  double ynew = point_to_rotate.x * s + point_to_rotate.y * c;

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