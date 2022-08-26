#include "Math.hpp"

#include <cmath>

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

}  // namespace math