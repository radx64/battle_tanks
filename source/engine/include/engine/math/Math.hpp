#pragma once

#include <cstddef>
#include <cstdint>
#include <numeric>
#include <vector>

#include "engine/Vector2.hpp"

namespace engine::math
{
float distance(float x1, float y1, float x2, float y2);
float distance(const engine::Vector2f& a, const engine::Vector2f& b);
float dotProduct(float x1, float y1, float x2, float y2);
float crossProduct(float x1, float y1, float x2, float y2);
float normalizeVector(engine::Vector2f& vector);
float signedFmod(float a, float n);
float degreeToRadians(float degree);
float radiansToDegree(float radians);
engine::Vector2f rotatePoint(engine::Vector2f pointToRotate, float angle, engine::Vector2f pivot);
engine::Vector2f lerp(const engine::Vector2f& a, const engine::Vector2f& b, float t);
float lerp(const float a, const float b, float t);

template <typename T>
class Average
{
public:
    Average (const size_t windowSize);
    T calculate(T newValue);

    using result_type = T;

    const std::vector<T>& measurements() const;

protected:
    size_t windowSize_{};
    std::vector<T> measurements_{};
    size_t currentIndex_{};
};

}  // namespace engine::math
