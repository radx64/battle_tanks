template <typename T>
inline Vector2<T>::Vector2() 
: x(0), y(0)
{}

template <typename T>
inline Vector2<T>::Vector2(const T x, const T y)
: x(x), y(y)
{}

template <typename T>
template <typename U>
inline Vector2<T>::Vector2(const Vector2<U>& vector)
: x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y))
{}

template <typename T>
inline Vector2<T> operator -(const Vector2<T>& vector)
{
    return Vector2<T>(-vector.x, -vector.y);
}

template <typename T>
inline Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right)
{
    left.x += right.x;
    left.y += right.y;

    return left;
}

template <typename T>
inline Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;

    return left;
}

template <typename T>
inline Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(left.x + right.x, left.y + right.y);
}

template <typename T>
inline Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(left.x - right.x, left.y - right.y);
}

template <typename T>
inline Vector2<T> operator *(const Vector2<T>& left, T scalar)
{
    return Vector2<T>(left.x * scalar, left.y * scalar);
}

template <typename T>
inline Vector2<T> operator *(T scalar, const Vector2<T>& right)
{
    return Vector2<T>(right.x * scalar, right.y * scalar);
}

template <typename T>
inline Vector2<T>& operator *=(Vector2<T>& left, T scalar)
{
    left.x *= scalar;
    left.y *= scalar;

    return left;
}

template <typename T>
inline Vector2<T> operator /(const Vector2<T>& left, T scalar)
{
    return Vector2<T>(left.x / scalar, left.y / scalar);
}

template <typename T>
inline Vector2<T>& operator /=(Vector2<T>& left, T scalar)
{
    left.x /= scalar;
    left.y /= scalar;

    return left;
}

template <typename T>
inline bool operator ==(const Vector2<T>& left, const Vector2<T>& right)
{
    return (left.x == right.x) && (left.y == right.y);
}

template <typename T>
inline bool operator !=(const Vector2<T>& left, const Vector2<T>& right)
{
    return !(left == right);
}
