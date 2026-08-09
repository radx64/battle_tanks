#pragma once

namespace engine
{

template <typename T>
class Vector2
{
public:
    Vector2();

    Vector2(const T x, const T y);

    template <typename U>
    explicit Vector2(const Vector2<U>& vector);

    T x;
    T y;
};

template <typename T>
Vector2<T> operator -(const Vector2<T>& vector);

template <typename T>
Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T> operator *(const Vector2<T>& left, T scalar);

template <typename T>
Vector2<T> operator *(T scalar, const Vector2<T>& right);

template <typename T>
Vector2<T>& operator *=(Vector2<T>& left, T scalar);

template <typename T>
Vector2<T> operator /(const Vector2<T>& left, T scalar);

template <typename T>
Vector2<T>& operator /=(Vector2<T>& left, T scalar);

template <typename T>
bool operator ==(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
bool operator !=(const Vector2<T>& left, const Vector2<T>& right);

#include "engine/Vector2.inl"

using Vector2i = engine::Vector2<int>;
using Vec2i = Vector2i;
using Vector2u = engine::Vector2<unsigned int>;
using Vec2u = Vector2u;
using Vector2f = engine::Vector2<float>;
using Vec2f = Vector2f;

}  // namespace engine
