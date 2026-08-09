#pragma once

#include <algorithm>

#include "engine/Vector2.hpp"

namespace engine
{
template <typename T>
class Rect
{
public:
    Rect();

    Rect(const T left, const T top, const T width, const T height);

    Rect(const Vector2<T>& position, const Vector2<T>& size);

    template <typename U>
    explicit Rect(const Rect<U>& rect);

    bool contains(const T x, const T y) const;

    bool contains(const Vector2<T>& point) const;

    bool intersects(const Rect<T>& rectangle) const;

    bool intersects(const Rect<T>& rectangle, Rect<T>& intersection) const;

    engine::Vector2<T> getPosition() const;

    engine::Vector2<T> getSize() const;

    T left;
    T top;
    T width;
    T height;
};

template <typename T>
bool operator ==(const Rect<T>& left, const Rect<T>& right);

template <typename T>
bool operator !=(const Rect<T>& left, const Rect<T>& right);

#include "engine/Rect.inl"

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

}  // namespace engine
